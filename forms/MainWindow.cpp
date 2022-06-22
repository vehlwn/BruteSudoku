#include "MainWindow.h"

#include "forms/TimeChartForm.h"
#include "src/Solvers/IterativeSolver.h"
#include "src/Solvers/RecursiveSolver.h"
#include "src/SudokuErrors.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

namespace {
void setPlainTextCursorAndFocus(QPlainTextEdit* const edit, const int pos)
{
    QTextCursor c = edit->textCursor();
    c.setPosition(pos);
    c.setPosition(pos + 1, QTextCursor::KeepAnchor);
    edit->setTextCursor(c);
    edit->setFocus();
}
/*
void screenShotWidget(QWidget* w)
{
    QPixmap    pix = w->grab();
    static int i;
    pix.save(QString{"out-%1.png"}.arg(i++));
}
*/
} // namespace

using vehlwn::SolvingThread;
using vehlwn::SudokuParserError;
using vehlwn::SudokuTable;
using vehlwn::SudokuTableError;
using vehlwn::Solvers::BaseSudokuSolver;
using vehlwn::Solvers::IterativeSolver;
using vehlwn::Solvers::RecursiveSolver;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui{new Ui::MainWindow}
{
    ui->setupUi(this);
    setWindowTitle("BruteSudoku");
    clear();

    m_solvingThread = new SolvingThread{this};
    connect(m_solvingThread, &SolvingThread::started, this, [this] {
        this->ui->pushButtonSolve->setText("Stop");
        this->ui->lineEditResult->setText("In progress...");
    });
    connect(m_solvingThread, &SolvingThread::goodResult, this,
        [this](const SudokuTable& solvedTable, std::uintmax_t nstime) {
            this->ui->lineEditResult->setText(QString{"%1 s (%2 ms, %3 ns)"}
                                                  .arg(nstime / 1.e9)
                                                  .arg(nstime / 1.e6)
                                                  .arg(nstime));
            this->m_solvedTable = solvedTable;
            const auto textSolution = QString::fromStdString(
                this->m_solvedTable.toString(SudokuTable::Indented));
            this->ui->plainTextEditSolved->setPlainText(textSolution);
        });
    connect(m_solvingThread, &SolvingThread::errorResult, this,
        [this](const QString& msg) { this->ui->lineEditResult->setText(msg); });
    connect(m_solvingThread, &SolvingThread::progressTable, this,
        [this](const QString& partiallySolved) {
            this->ui->plainTextEditSolved->setPlainText(partiallySolved);
        });
    connect(m_solvingThread, &SolvingThread::finished, this,
        [this] { this->ui->pushButtonSolve->setText("Solve"); });
}

MainWindow::~MainWindow()
{
    delete ui;
    m_solvingThread->stop();
    m_solvingThread->wait();
}

void MainWindow::on_actionExit_triggered()
{
    // Exit.
    close();
}

void MainWindow::on_actionLoadTable_triggered()
try
{
    // Read table from a file.
    m_solvingThread->stop();
    static QString oldFileName;
    const QString  fileName = QFileDialog::getOpenFileName(
        this, "Load table", oldFileName, "Text files (*.txt);;All files (*.*)");
    if(fileName.isEmpty())
        return;

    oldFileName = fileName;
    QFile file{fileName};
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::runtime_error("Cannot open " + fileName.toStdString());

    std::string s;
    {
        std::vector<char> buf(1024 * 1024);
        const qint64      num = file.read(buf.data(), buf.size());
        if(-1 == num)
            throw std::runtime_error("Cannot read " + fileName.toStdString());
        s = {buf.begin(), buf.end()};
    }
    m_problemTable.parse(s);
    ui->plainTextEditProblem->setPlainText(
        QString::fromStdString(m_problemTable.toString(SudokuTable::Indented)));
}
catch(const std::exception& ex)
{
    QMessageBox::critical(this, "Error", ex.what());
}

std::unique_ptr<BaseSudokuSolver> MainWindow::getCheckedSolver() const
{
    // Factory to choose solver implementation.
    std::unique_ptr<BaseSudokuSolver> ret;
    if(ui->radioButtonRecursive->isChecked())
        ret = std::make_unique<RecursiveSolver>();
    else if(ui->radioButtonIterative->isChecked())
        ret = std::make_unique<IterativeSolver>();
    return ret;
}

void MainWindow::clear()
{
    m_problemTable.reset();
    m_solvedTable.reset();
    ui->plainTextEditProblem->setPlainText(
        QString::fromStdString(m_problemTable.toString(SudokuTable::Indented)));
    ui->plainTextEditSolved->clear();
    ui->lineEditResult->clear();
}

void MainWindow::on_actionGrammar_triggered()
{
    QMessageBox::information(this, "Grammar", R"(E -> (\s*([1-9]|'.')\s*){81})");
}

void MainWindow::on_pushButtonSolve_clicked()
{
    // Solve.
    if(m_solvingThread->isRunning())
        m_solvingThread->stop();
    else
        try
        {
            const std::string inputStr =
                ui->plainTextEditProblem->toPlainText().toStdString();
            m_problemTable.parse(inputStr);
            ui->plainTextEditProblem->setPlainText(
                QString::fromStdString(m_problemTable.toString(SudokuTable::Indented)));

            auto solver = getCheckedSolver();
            if(!solver)
                throw std::runtime_error{"Selected solver not implemented"};
            solver->table(m_problemTable);
            m_solvingThread->solver(std::move(solver));
            m_solvingThread->start();
        }
        catch(const SudokuParserError& ex)
        {
            QMessageBox::critical(this, "Parser error", ex.what());
            setPlainTextCursorAndFocus(ui->plainTextEditProblem, ex.pos());
        }
        catch(const SudokuTableError& ex)
        {
            const QString msg = (QString{ex.what()} + "\nrow = %1, col = %2, val = %3")
                                    .arg(ex.row())
                                    .arg(ex.col())
                                    .arg(ex.val());
            setPlainTextCursorAndFocus(ui->plainTextEditProblem, ex.pos());
            QMessageBox::critical(this, "Table error", msg);
        }
        catch(const std::exception& ex)
        {
            QMessageBox::critical(this, "Error", ex.what());
        }
}

void MainWindow::on_pushButtonClear_clicked()
{
    m_solvingThread->stop();
    clear();
}

void MainWindow::on_actionSaveSolution_triggered()
try
{
    if(m_solvingThread->isRunning())
        throw std::runtime_error("Solving in progress. Press Stop.");

    // Save.
    static QString oldFielName = "solution";
    const QString  fileName = QFileDialog::getSaveFileName(
        this, "Save solution", oldFielName, "Text files (*.txt);;All files (*.*)");
    if(fileName.isEmpty())
        return;
    oldFielName = fileName;

    QFile file{fileName};
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        throw std::runtime_error("Cannot open " + fileName.toStdString());
    const std::string s = m_solvedTable.toString(SudokuTable::Compact);
    const qint64      num = file.write(s.data(), s.size());
    if(-1 == num)
        throw std::runtime_error("Cannot write to " + fileName.toStdString());
}
catch(const std::exception& ex)
{
    QMessageBox::critical(this, "Error", ex.what());
}

void MainWindow::on_actionSolutionTimeChart_triggered()
{
    static TimeChartForm* chartForm;
    if(!chartForm)
    {
        chartForm = new TimeChartForm{this};
        chartForm->setWindowFlags(Qt::Window);
        connect(m_solvingThread, &SolvingThread::goodResult, chartForm,
            [](const SudokuTable& solvedTable, std::uintmax_t nstime) {
                (void)solvedTable;
                emit chartForm->append(nstime / 1.e9);
            });
    }
    chartForm->show();
}
