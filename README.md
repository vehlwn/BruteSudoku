# BruteSudoku29

This program solve 9x9 [sudoku](https://en.wikipedia.org/wiki/Sudoku) with recursive descent ([backtracking](https://en.wikipedia.org/wiki/Sudoku_solving_algorithms#Sudoku_brute_force)) without explicit recursion. Candidates for each cell are chosen randomly, so for one table it can find several solutions (if ones exist). My algorithm can be generalized to arbitrary size tables.

The program is written in C++11 and depends only on Qt external libraries for interface.

## Algorithm:

Input: 
- grid - initial table of numbers 9x9 (or any other square size), probable not filled fully.

Output: 
- same grid from input, filled with numbers, representing soluton, if one exists; or error, if grid is filled wrong.

Variables: 
- candidates - map &lt;int, vector &lt; int &gt;&gt;, containing vectors of potential candidates for each cell;
- stack &lt; int &gt; - stack, representing recursive calls, containing coordinates of cell, filled on each step of recursion.

### Pseudocode:
```
1. While there is emty cell in grid, do:  
1.1. Put this emty cell has coordinate pos.  
1.2. Put flag isCellChanged := false.  
1.3. Put vector values := candidates[pos] - reference to candidates for cell pos.  
1.4. Initilize values with random integers from minimun number in grid of this size 
     to maximun (i.e. from 1 to 9).  
1.5. While values is not empty, do:  
1.5.1. Put val := values.back;  
1.5.2. Remove back element: values.pop_back.  
1.5.3. If val can be placed at pos, accordingly to rules of sudoku:  
1.5.3.1. Assign grid[pos] := val.  
1.5.3.2. Push current coordinate to stack (imitate recursive call): stack.push(pos).  
1.5.3.3. Assign isCellChanged := true.  
1.5.3.4. Break loop 1.5.  
1.6. If isCellChanged = false: at pos none of digits is allowed. Unwind stack and 
     try change previous values (perform backtracking):  
1.6.1. While stack is not empty, do:  
1.6.1.1. Put tmpPos := stack.top.  
1.6.1.2. Remove element from stack: stack.pop.  
1.6.1.3. Put isCellChanged := false.  
1.6.1.4. Put vector tmpValues := candidates[tmpPos] - reference to candidates for cell 
         tmpPos. These candidates already filled at step 1.4.  
1.6.1.5. While tmpValues is not empty, do:  
1.6.1.5.1. Put val := tmpValues.back;  
1.6.1.5.2. Remove back element: tmpValues.pop_back.  
1.6.1.5.3. If val can be placed at tmpPos, accordingly to rules of sudoku:  
1.6.1.5.3.1. Assign grid[tmpPos] := val.  
1.6.1.5.3.2. Push current coordinate to stack (imitate recursive call): stack.push(tmpPos).  
1.6.1.5.3.3. Assign isCellChanged := true.  
1.6.1.5.3.4. Break loop 1.6.1.5.  
1.6.1.6. If isCellChanged = true:  
1.6.1.6.1. Break loop 1.6.1 - we changed some previous cell.  
1.6.1.7. Else (isCellChanged = false):  
1.6.1.7.1. Assign grid[tmpPos] := empty - try unwind more one level of recursion.  
1.6.2. If stack is empty - we've unwound all levels of recursion and couldn't find appopriate value:  
1.6.2.1. Stop algorithm. Raise error 'grid filled wrong'.  
2. Return filled grid.  
```
To discard knowingly unacceptable candidates from beginning of the algorithm, I choose start cell, from which emty cell should be found, from the most filled row in a grid.

More details in [this file](src/SudokuSolver.cpp).

## Examples
![alt text](examples/example1.png)
![alt text](examples/example2.png)
![alt text](examples/example3.png)
