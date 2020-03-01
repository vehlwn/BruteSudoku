#include "SolvingThread.h"

namespace vehlwn
{
SolvingThread::SolvingThread(QObject* parent)
    : base{parent}
{
}

SolvingThread::~SolvingThread() = default;

void SolvingThread::solver(std::unique_ptr<Solvers::BaseSudokuSolver> solver)
{
    m_solver = std::move(solver);
    connect(m_solver.get(), &Solvers::BaseSudokuSolver::errorResult, this,
        &SolvingThread::errorResult);
    connect(m_solver.get(), &Solvers::BaseSudokuSolver::progressTable, this,
        &SolvingThread::progressTable);
    connect(m_solver.get(), &Solvers::BaseSudokuSolver::goodResult, this,
        &SolvingThread::goodResult);
}

void SolvingThread::run()
{
    try
    {
        auto nstime = m_solver->nstimeSolve();
        emit goodResult(m_solver->table(), nstime);
    }
    catch(const std::exception& ex)
    {
        emit errorResult(ex.what());
    }
}

void SolvingThread::stop()
{
    if(m_solver && isRunning())
        m_solver->stop();
}
} // namespace vehlwn
