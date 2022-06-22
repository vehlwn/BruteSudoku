#include "IterativeSolver.h"

#include <map>
#include <stack>
#include <vector>

namespace vehlwn { namespace Solvers {
// Non recursive iterative backtracking.
void IterativeSolver::solveImpl()
{
    m_t1 = std::chrono::steady_clock::now();
    using namespace std::literals;

    std::size_t                     emptyCell = mostFilledRow() * SudokuTable::WIDTH;
    std::map<int, std::vector<int>> candidates;
    std::stack<int>                 stas;
    while(!m_stopped)
    {
        bool b;
        std::tie(emptyCell, b) = getEmptyCell(emptyCell);
        if(!b)
            break;
        auto& values = candidates[emptyCell];
        values = genCandidates();
        const auto tryCandidates = [this, &stas](auto& values, const std::size_t pos) {
            while(!m_stopped && !values.empty())
            {
                const auto val = values.back();
                values.pop_back();
                if(this->m_table.trySet(pos, val))
                {
                    auto t2 = std::chrono::steady_clock::now();
                    if(t2 - m_t1 > PROGRESS_TABLE_DELAY)
                    {
                        // Prevent flooding main thread with too many progress messages.
                        const QString s = QString::fromStdString(m_table.toString());
                        emit          progressTable(s);
                        m_t1 = t2;
                    }
                    stas.push(pos);
                    return true;
                }
            }
            return false;
        };

        if(tryCandidates(values, emptyCell))
            continue;

        // At pos none of digits is allowed. Unwind stack and try
        // change previous values.
        while(!m_stopped && !stas.empty())
        {
            const auto tmpPos = stas.top();
            stas.pop();
            auto& tmpValues = candidates[tmpPos];
            if(tryCandidates(tmpValues, tmpPos))
                break;
            else
            {
                m_table(tmpPos) = SudokuTable::EMPTY_NUM;
                emptyCell = tmpPos;
            }
        }
        if(stas.empty())
            throw std::runtime_error("No solution");
    }
    if(m_stopped)
        throw std::runtime_error{"Stopped"};
}
}} // namespace vehlwn::Solvers
