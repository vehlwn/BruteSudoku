#include "BaseSudokuSolver.h"

#include "src/StdRandom.h"

#include <algorithm>
#include <chrono>

namespace vehlwn { namespace Solvers {
std::uintmax_t BaseSudokuSolver::nstimeSolve()
{
    const auto t1 = std::chrono::steady_clock::now();
    solve();
    const auto t2 = std::chrono::steady_clock::now();
    return static_cast<std::uintmax_t>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count());
}

void BaseSudokuSolver::parse(const std::string_view s)
{
    m_table.parse(s);
}

const SudokuTable& BaseSudokuSolver::table() const
{
    return m_table;
}

void BaseSudokuSolver::table(const SudokuTable& t)
{
    m_table = t;
}

void BaseSudokuSolver::solve()
{
    m_stopped = false;
    solveImpl();
}

void BaseSudokuSolver::stop()
{
    m_stopped = true;
}

std::size_t BaseSudokuSolver::mostFilledRow() const
{
    std::size_t res = 0, num2 = 0;
    for(std::size_t i = 0; i != SudokuTable::WIDTH; i++)
    {
        std::size_t num = 0;
        for(std::size_t j = 0; j != SudokuTable::WIDTH; j++)
            if(m_table(i, j) != SudokuTable::EMPTY_NUM)
                num++;
        if(num > num2)
        {
            res = i;
            num2 = num;
        }
    }
    return res;
}

std::pair<std::size_t, bool> BaseSudokuSolver::getEmptyCell(const std::size_t start) const
{
    std::size_t pos = start;
    do
    {
        if(m_table(pos) == SudokuTable::EMPTY_NUM)
            return {pos, true};
        pos++;
        if(SudokuTable::SIZE == pos)
            pos = 0;
    } while(pos != start);
    return {-1, false};
}

std::vector<int> BaseSudokuSolver::genCandidates()
{
    std::vector<int> ret{
        SudokuTable::VALID_DIGITS.begin(), SudokuTable::VALID_DIGITS.end()};
    std::shuffle(ret.begin(), ret.end(), StdRandom::gen);
    return ret;
}
}} // namespace vehlwn::Solvers
