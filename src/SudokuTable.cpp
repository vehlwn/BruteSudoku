#include "SudokuTable.h"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace vehlwn {
SudokuTable::SudokuTable()
{
    reset();
}

void SudokuTable::reset()
{
    m_grid.fill(EMPTY_NUM);
    m_parsedPositions.fill(0);
}

void SudokuTable::parse(const std::string_view s)
{
    reset();
    std::size_t i = 0;
    using namespace std::string_literals;
    for(std::size_t pos = 0; pos != s.size(); pos++)
    {
        if(m_grid.size() == i)
            break;
        else if(std::isspace(s[pos]))
            continue;
        else if(s[pos] == EMPTY_CHAR)
        {
            m_grid[i] = EMPTY_NUM;
            m_parsedPositions[i] = pos;
            i++;
        }
        else if(std::isdigit(s[pos]))
        {
            const int num = s[pos] - '0';
            if(!std::binary_search(VALID_DIGITS.begin(), VALID_DIGITS.end(), num))
                throw SudokuParserError("Illegal character '"s + s[pos] + "'", pos);
            m_grid[i] = num;
            m_parsedPositions[i] = pos;
            i++;
        }
        else
            throw SudokuParserError("Illegal character '"s + s[pos] + "'", pos);
    }
    verifyAll(true);
}

bool SudokuTable::trySet(const std::size_t emptyCell, const int val)
{
    if(isLegal(emptyCell, val))
    {
        (*this)(emptyCell) = val;
        return true;
    }
    return false;
}

std::string SudokuTable::toString(const StringFormat format) const
{
    std::ostringstream os;
    switch(format)
    {
    case StringFormat::Compact:
    {
        for(const int n : m_grid)
            if(EMPTY_NUM == n)
                os << EMPTY_CHAR;
            else
                os << n;
        break;
    }
    case StringFormat::Indented:
    {
        int i = 0;
        for(const int n : m_grid)
        {
            if(EMPTY_NUM == n)
                os << EMPTY_CHAR;
            else
                os << n;
            os << " ";
            // 1 block vertically
            if((i + 1) % (WIDTH * NUN_BLOCKS) == 0)
                os << "\n\n";
            // 1 line
            else if((i + 1) % WIDTH == 0)
                os << "\n";
            // 1 block within 1 line
            else if((i + 1) % BLOCK_WIDTH == 0)
                os << "  ";
            i++;
        }
        break;
    }
    }
    return os.str();
}
int& SudokuTable::operator()(const std::size_t idx)
{
    return m_grid[idx];
}
int& SudokuTable::operator()(const std::size_t idx, const std::size_t jdx)
{
    return m_grid[idx * WIDTH + jdx];
}
int SudokuTable::operator()(const std::size_t idx) const
{
    return m_grid[idx];
}
int SudokuTable::operator()(const std::size_t idx, const std::size_t jdx) const
{
    return m_grid[idx * WIDTH + jdx];
}

bool SudokuTable::isLegal(const std::size_t pos, const int val, const bool doRaise) const
{
    const auto row = pos / WIDTH;
    const auto col = pos % WIDTH;
    const bool ret = isRowOk(row, col, val, doRaise) && isColOk(row, col, val, doRaise)
        && isSqrOk(row, col, val, doRaise);
    return ret;
}
bool SudokuTable::verifyAll(const bool doRaise) const
{
    for(std::size_t i = 0; i != m_grid.size(); i++)
        if(m_grid[i] != EMPTY_NUM && !isLegal(i, m_grid[i], doRaise))
            return false;
    return true;
}
bool SudokuTable::isRowOk(
    const std::size_t row, const std::size_t col, const int val, const bool doRaise) const
{
    for(std::size_t i = 0; i != WIDTH; i++)
    {
        const auto num = (*this)(row, i);
        if(i != col && num == val && num != EMPTY_NUM)
        {
            if(doRaise)
                throw SudokuRowError(
                    row + 1, col + 1, num, m_parsedPositions[row * WIDTH + col]);
            return false;
        }
    }
    return true;
}
bool SudokuTable::isColOk(
    const std::size_t row, const std::size_t col, const int val, const bool doRaise) const
{
    for(std::size_t i = 0; i != WIDTH; i++)
    {
        const auto num = (*this)(i, col);
        if(i != row && num == val && num != EMPTY_NUM)
        {
            if(doRaise)
                throw SudokuColumnError(
                    row + 1, col + 1, num, m_parsedPositions[row * WIDTH + col]);
            return false;
        }
    }
    return true;
}
bool SudokuTable::isSqrOk(
    const std::size_t row, const std::size_t col, const int val, const bool doRaise) const
{
    const auto row_corner = row / BLOCK_WIDTH * BLOCK_WIDTH;
    const auto col_corner = col / BLOCK_WIDTH * BLOCK_WIDTH;

    for(std::size_t i = row_corner; i != row_corner + BLOCK_WIDTH; i++)
        for(std::size_t j = col_corner; j != col_corner + BLOCK_WIDTH; j++)
        {
            const auto num = (*this)(i, j);
            if((i != row || j != col) && num == val && num != EMPTY_NUM)
            {
                if(doRaise)
                    throw SudokuSquareError(
                        row + 1, col + 1, num, m_parsedPositions[row * WIDTH + col]);
                return false;
            }
        }
    return true;
}
} // namespace vehlwn
