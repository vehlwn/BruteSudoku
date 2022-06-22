#pragma once

#include "SudokuErrors.h"

#include <array>
#include <string>

namespace vehlwn {
// WIDTH**2 table with numbers.
class SudokuTable
{
public:
    enum StringFormat
    {
        Compact,
        Indented
    };
    SudokuTable();
    int         operator()(const std::size_t idx) const;
    int         operator()(const std::size_t idx, const std::size_t jdx) const;
    int&        operator()(const std::size_t idx);
    int&        operator()(const std::size_t idx, const std::size_t jdx);
    std::string toString(const StringFormat format = Indented) const;
    void        parse(const std::string_view s);
    bool        trySet(const std::size_t emptyCell, const int val);
    void        reset();

    static constexpr int        EMPTY_NUM = 0;
    static constexpr int        WIDTH = 9;
    static constexpr int        SIZE = WIDTH * WIDTH;
    static constexpr std::array VALID_DIGITS = {1, 2, 3, 4, 5, 6, 7, 8, 9};

private:
    bool isLegal(const std::size_t pos, const int val, const bool doRaise = false) const;
    bool verifyAll(const bool doRaise = false) const;
    bool isRowOk(const std::size_t row, const std::size_t col, const int val,
        const bool doRaise = false) const;
    bool isColOk(const std::size_t row, const std::size_t col, const int val,
        const bool doRaise = false) const;
    bool isSqrOk(const std::size_t row, const std::size_t col, const int val,
        const bool doRaise = false) const;

    static constexpr char EMPTY_CHAR = '.';
    static constexpr int  BLOCK_WIDTH = 3;
    static constexpr int  NUN_BLOCKS = 3;
    std::array<int, SIZE> m_grid;
    std::array<int, SIZE> m_parsedPositions;
};
} // namespace vehlwn
