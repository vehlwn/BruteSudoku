#pragma once

#include <exception>
#include <string>

namespace vehlwn
{
// Calss for syntax error in input string.
class SudokuParserError : public std::exception
{
public:
    SudokuParserError(const std::string& msg, const std::size_t pos)
        : m_msg{msg}
        , m_pos{pos}
    {
    }
    virtual const char* what() const noexcept override
    {
        return m_msg.data();
    }
    std::size_t pos() const noexcept
    {
        return m_pos;
    }

private:
    std::string m_msg;
    std::size_t m_pos;
};

// Base class for invalid gidit in row, column, square.
class SudokuTableError : public std::exception
{
public:
    SudokuTableError(const int row, const int col, const int val, const std::size_t pos)
        : m_row{row}
        , m_col{col}
        , m_val{val}
        , m_pos{pos}
    {
    }
    int row() const noexcept
    {
        return m_row;
    }
    int col() const noexcept
    {
        return m_col;
    }
    int val() const noexcept
    {
        return m_val;
    }
    std::size_t pos() const noexcept
    {
        return m_pos;
    }

private:
    int         m_row;
    int         m_col;
    int         m_val;
    std::size_t m_pos;
};

// Error digit in a row.
class SudokuRowError : public SudokuTableError
{
public:
    SudokuRowError(const int row, const int col, const int val, const std::size_t pos)
        : SudokuTableError{row, col, val, pos}
    {
    }
    virtual const char* what() const noexcept override
    {
        return "Invalid row value";
    }
};

// Error digit in a column.
class SudokuColumnError : public SudokuTableError
{
public:
    SudokuColumnError(const int row, const int col, const int val, const std::size_t pos)
        : SudokuTableError{row, col, val, pos}
    {
    }
    virtual const char* what() const noexcept override
    {
        return "Invalid column value";
    }
};

// Error digit in a square.
class SudokuSquareError : public SudokuTableError
{
public:
    SudokuSquareError(const int row, const int col, const int val, const std::size_t pos)
        : SudokuTableError{row, col, val, pos}
    {
    }
    virtual const char* what() const noexcept override
    {
        return "Invalid square value";
    }
};
} // namespace vehlwn
