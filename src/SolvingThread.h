#pragma once

#include "src/Solvers/BaseSudokuSolver.h"

#include <QObject>
#include <QString>
#include <QThread>
#include <memory>

namespace vehlwn
{
class SolvingThread : public QThread
{
    Q_OBJECT
    using base = QThread;

public:
    SolvingThread(QObject* parent = nullptr);
    ~SolvingThread();
    void solver(std::unique_ptr<Solvers::BaseSudokuSolver> solver);

private:
    std::unique_ptr<Solvers::BaseSudokuSolver> m_solver;

protected:
    virtual void run() override;

signals:
    void errorResult(const QString&);
    void progressTable(const QString&);
    void goodResult(const SudokuTable&, std::uintmax_t);

public slots:
    void stop();
};
} // namespace vehlwn
