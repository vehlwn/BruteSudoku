#pragma once

#include "BaseSudokuSolver.h"

#include <chrono>

namespace vehlwn { namespace Solvers {
// Non recursive iterative backtracking.
class IterativeSolver : public BaseSudokuSolver
{
    using base = BaseSudokuSolver;

protected:
    virtual void solveImpl() override;

private:
    std::chrono::steady_clock::time_point m_t1;
};
}} // namespace vehlwn::Solvers
