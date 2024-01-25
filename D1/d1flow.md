# Discussion 01 (Jan 24, 2023)


## Demonstrate Project 0

Link: `https://git.doit.wisc.edu/cdis/cs/courses/cs537/spring24/public/p0`

Steps:
1. Remote connection to CST
    - ssh
    - GlobalProtect
2. Basic Terminal Commands
    - `pwd` - present working directory
    - `ls` - list
    - `cd` - change directory
    - `mkdir` - make directory
    - `mv` - move
    - `cp` - copy
    - `cat` - concatenate files and print on the standard output
    - `grep` - print lines of a file that match patterns

3. CS Department's File Directory Structure

4. Cloning and Starting Your Projects

5. Creating and Editing Code with `micro`
    - https://cheatography.com/mynocksonmyfalcon/cheat-sheets/micro-text-editor/
    - `Ctrl + q`: Close current file (quits micro if this is the last file open)
    - `Ctrl + s`: Save current file
    - `Ctrl + e`: Open a command prompt for running commands( e.g., `hsplit`)
6. `gcc`, `gdb`, `Makefile`, `man` pages

7. Running Tests
8. Handing In Your Project Solution
9. Project Results



## Project 01



### GDB (GNU Debugger)

- buggy.c
- buggyfactorial.c

#### Compile 

**Flags:**

- $ `gcc -o hello hello-world.c`                # `-o`:       provide executable name

- $ `gcc -Wall hello-world.c`                   # `-Wall`:    show better warnings

- $ `gcc -Werror hello-world.c`                 # `-Werror`:  show warnings as errors

- $ `gcc -O hello-world.c`                      # `-O`:       optimize code

- $ `gcc -g hello-world.c`                      # `-g`:       debugging using gdb


$ `gcc -g buggy.c -o buggy`


#### Run gdb

Ref: https://www.cs.umd.edu/~srhuang/teaching/cmsc212/gdb-tutorial-handout.pdf

- $ `gdb buggy`

or 

- $ `gdb`

- $ `file <executable-name>`

#### Debug


$ `run` 

$ `continue` or `c`

**breakpoints**

$ `break <func-name>` e.g., `break main`

$ `break buggy.c:11`

$ `break buggy.c:15`

$ `next` or `n`

$ `step` or `s`

$ `print a`

$ `print {a, b}`

$ `print/x a>`

**watch and display**

$ `watch a`

$ `watch {a,b}`

$ `watch p`

$ `display {a, f}`

**info**

$ `frame`

$ `list`

$ `info breakpoints` or `info b`

$ `info args`

$ `info watchpoints` or `info watch`

**help**

$ `help`

$ `help run`

**args**

$ `set args <arg1> <arg2> ...`

$ `set args`

$ `run <arg1> <arg2>`

**delete breakpoints**

$ `delete 1` # deletes the first breakpoint shown in `info b`

**quit**

$ `quit`

cheat-sheet: https://people.computing.clemson.edu/~chochri/gdb-basics.pdf



### `man` Command

$ `man ls`

$ `man printf`

$ `man tan`

$ `man open`

$ `man 2 open`

$ `man -k file`

### File manipulation

file.c shows example of
  - `fopen`
  - `fclose`
  - `perror`
  - `fgetc`

mycat.c shows example of
  - `fgets`
  - `fseek`


### String manipulation

tokenize.c shows example of
  - `strtok`
  - `strcmp`








