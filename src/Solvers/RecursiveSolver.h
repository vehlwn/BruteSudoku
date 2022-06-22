#pragma once

#include "BaseSudokuSolver.h"

#include <chrono>

namespace vehlwn { namespace Solvers {
// Recursive backtracking.
class RecursiveSolver : public BaseSudokuSolver
{
    using base = BaseSudokuSolver;

private:
    bool solveRecursive(std::size_t emptyCell);

protected:
    virtual void solveImpl() override;

private:
    std::chrono::steady_clock::time_point m_t1;
};
}} // namespace vehlwn::Solvers
