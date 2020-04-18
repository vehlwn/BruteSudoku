#pragma once

#include "src/Solvers/BaseSudokuSolver.h"
#include "src/SolvingThread.h"

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSolve_clicked();
    void on_actionExit_triggered();
    void on_actionLoadTable_triggered();
    void on_actionGrammar_triggered();
    void on_pushButtonClear_clicked();
    void on_actionSaveSolution_triggered();

    void on_actionSolutionTimeChart_triggered();

private:
    std::unique_ptr<vehlwn::Solvers::BaseSudokuSolver> getCheckedSolver() const;
    void clear();

    Ui::MainWindow* ui{};
    vehlwn::SudokuTable m_problemTable, m_solvedTable;
    vehlwn::SolvingThread* m_solvingThread{};
};
