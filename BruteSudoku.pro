#-------------------------------------------------
#
# Project created by QtCreator 2017-12-28T22:59:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

TARGET = BruteSudoku
TEMPLATE = app

CONFIG += c++17 strict_c++

SOURCES += src/main.cpp \
	forms/TimeChartForm.cpp \
	src/LinearChartWithAverage.cpp \
	src/Solvers/BaseSudokuSolver.cpp \
	src/SolvingThread.cpp \
	src/StdRandom.cpp \
	src/SudokuErrors.cpp \
	src/SudokuTable.cpp \
		forms/MainWindow.cpp \
	src/Solvers/IterativeSolver.cpp \
	src/Solvers/RecursiveSolver.cpp

HEADERS  += forms/MainWindow.h \
	forms/TimeChartForm.h \
	src/LinearChartWithAverage.h \
	src/Solvers/BaseSudokuSolver.h \
	src/SolvingThread.h \
	src/StdRandom.h \
	src/SudokuErrors.h \
	src/SudokuTable.h \
	src/Solvers/IterativeSolver.h \
	src/Solvers/RecursiveSolver.h

FORMS    += forms/MainWindow.ui \
	forms/TimeChartForm.ui



