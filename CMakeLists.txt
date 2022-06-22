cmake_minimum_required(VERSION 3.5)

project(BruteSudoku LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5 REQUIRED COMPONENTS 
    Widgets
    Charts
)

set(GUI_EXECUTABLE)
if(WIN32)
	set(GUI_EXECUTABLE WIN32)
elseif(APPLE)
	set(GUI_EXECUTABLE MACOSX_BUNDLE)
endif()

add_executable(
    ${PROJECT_NAME} ${GUI_EXECUTABLE}
    forms/MainWindow.cpp
    forms/MainWindow.h
    forms/MainWindow.ui
    forms/TimeChartForm.cpp
    forms/TimeChartForm.h
    forms/TimeChartForm.ui
    src/LinearChartWithAverage.cpp
    src/LinearChartWithAverage.h
    src/Solvers/BaseSudokuSolver.cpp
    src/Solvers/BaseSudokuSolver.h
    src/Solvers/IterativeSolver.cpp
    src/Solvers/IterativeSolver.h
    src/Solvers/RecursiveSolver.cpp
    src/Solvers/RecursiveSolver.h
    src/SolvingThread.cpp
    src/SolvingThread.h
    src/StdRandom.cpp
    src/StdRandom.h
    src/SudokuErrors.cpp
    src/SudokuErrors.h
    src/SudokuTable.cpp
    src/SudokuTable.h
    src/main.cpp
)
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt5::Widgets
    Qt5::Charts
)
