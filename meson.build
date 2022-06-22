project(
  'BruteSudoku', 'cpp',
  license: 'MIT',
  default_options: ['cpp_std=c++17'])

qt5 = import('qt5')
qt5_dep = dependency('qt5', modules: ['Widgets', 'Charts'])
inc = include_directories('src', 'forms')
qtprocessed = qt5.preprocess(
  moc_headers: [
    'forms/MainWindow.h',
    'forms/TimeChartForm.h',
    'src/LinearChartWithAverage.h',
    'src/SolvingThread.h',
    'src/Solvers/BaseSudokuSolver.h'],
  ui_files: ['forms/MainWindow.ui', 'forms/TimeChartForm.ui'],
  include_directories: inc)

exe = executable(
  'BruteSudoku',
  'src/main.cpp',
  'src/LinearChartWithAverage.cpp',
  'src/SolvingThread.cpp',
  'src/StdRandom.cpp',
  'src/SudokuErrors.cpp',
  'src/SudokuTable.cpp',
  'src/Solvers/BaseSudokuSolver.cpp',
  'src/Solvers/IterativeSolver.cpp',
  'src/Solvers/RecursiveSolver.cpp',
  'forms/MainWindow.cpp',
  'forms/TimeChartForm.cpp',
  qtprocessed,
  include_directories: inc,
  dependencies: qt5_dep)
