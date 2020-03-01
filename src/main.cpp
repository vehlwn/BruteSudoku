#include "forms/MainWindow.h"
#include "src/SudokuTable.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    // Register types for void BaseSudokuSolver::goodResult(const SudokuTable&,
    // std::uintmax_t) signal.
    qRegisterMetaType<vehlwn::SudokuTable>("SudokuTable");
    qRegisterMetaType<std::uintmax_t>("std::uintmax_t");
    QApplication a(argc, argv);
    MainWindow   w;
    w.show();
    return a.exec();
}
