#include "RecursiveSolver.h"

#include <tuple>

namespace vehlwn { namespace Solvers {
// Recursive backtracking.
void RecursiveSolver::solveImpl()
{
    m_t1 = std::chrono::steady_clock::now();
    if(!solveRecursive(mostFilledRow() * SudokuTable::WIDTH))
        throw std::runtime_error{"No solution"};
}

bool RecursiveSolver::solveRecursive(std::size_t emptyCell)
{
    if(m_stopped)
        throw std::runtime_error{"Stopped"};
    bool b;
    std::tie(emptyCell, b) = getEmptyCell(emptyCell);
    if(!b)
        return true;
    using namespace std::literals;
    for(const auto val : genCandidates())
        if(m_table.trySet(emptyCell, val))
        {
            const auto t2 = std::chrono::steady_clock::now();
            if(t2 - m_t1 > PROGRESS_TABLE_DELAY)
            {
                // Prevent flooding main thread with too many progress messages.
                const QString s = QString::fromStdString(m_table.toString());
                emit          progressTable(s);
                m_t1 = t2;
            }
            if(solveRecursive(emptyCell))
                return true;
            else
                m_table(emptyCell) = SudokuTable::EMPTY_NUM;
        }
    return false;
}
}} // namespace vehlwn::Solvers
