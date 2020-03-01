#pragma once

#include "src/SudokuTable.h"

#include <QObject>
#include <cstddef>
#include <string_view>
#include <vector>

namespace vehlwn { namespace Solvers {
// Parse sudoku table and provide intarface to
// different solvers.
class BaseSudokuSolver : public QObject
{
    Q_OBJECT
public:
    const SudokuTable& table() const;
    std::uintmax_t     nstimeSolve();
    void               parse(const std::string_view s);
    void               table(const SudokuTable& t);
    void               solve();

signals:
    void errorResult(const QString&);
    void goodResult(const SudokuTable&, std::uintmax_t);
    void progressTable(const QString&);

public slots:
    void stop();

protected:
    virtual void                 solveImpl() = 0;
    static std::vector<int>      genCandidates();
    std::pair<std::size_t, bool> getEmptyCell(const std::size_t start) const;
    std::size_t                  mostFilledRow() const;

    SudokuTable                            m_table;
    bool                                   m_stopped{false};
    static constexpr std::chrono::duration PROGRESS_TABLE_DELAY =
        std::chrono::milliseconds{100};
};
}} // namespace vehlwn::Solvers
