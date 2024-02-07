# Discussion 03

[Relevant project](https://git.doit.wisc.edu/cdis/cs/courses/cs537/spring24/public/p3/-/blob/main/instructions/instructions.md?ref_type=heads)

## Summary

- go through the project description
    CTRL C, CTRL Z

- Basic Shell
    - some linux commands
        - pipes
        - environment variables and shell variables
- ideas for creating a simple shell

## Basic Shell



### Some linux commands

- $ `echo hello`
- $ `cd ..`
- $ `cd D3`
- $ `sleep 10`

### Piping
  
- $ `cat helloworld | echo`
- $ `cat data.csv | grep green | sort | wc -l`

### Built-in cmds

- `cd`
- `history`

- `which gcc`

### Environment variables and shell variables

Shell variables are limited to the shell session in which they are defined. They are local to the shell or shell script and are not inherited by child processes.
Environment variables are passed from a parent process to child processes. They are available to any process started from the shell session, including subprocesses and programs executed within that session.

Shell variables are typically used for storing temporary data, configuration options specific to a shell session, or variables used within shell scripts.
Environment variables are often used for configuration options that need to be accessed by multiple processes or programs, such as system paths, user preferences, or settings for applications.

- $ `env` #  display a list of environment variables

    ```bash
    SHELL=/bin/bash
    COLORTERM=truecolor
    TERM_PROGRAM_VERSION=1.86.0
    HOSTNAME=
    LANGUAGE=en_US:
    PWD=/home/fariha/private/sp24/ta-cs537/dis/D3/examples
    MANPATH=:/opt/puppetlabs/puppet/share/man
    USER=fariha
    ```
- $ `printenv`

```bash
$ num=5
$ filename=a.txt
$ echo $num > $filename
$ ls
$ cat $filename
```
```bash
curr_path=$(pwd)
mkdir $curr_path/test_dir
```

## ideas for creating a simple shell

A simple shell is basically a `while` loop with `fork` and `exec`

### Fork

The C fork() function is a primary method of process creation of an operating 
system like Unix. 
Forking is the process of creating of duplicate process from an existing process.
The fork() is used for creating a new copy of the calling function. 
The newly created process is known as the Child process and the process 
from which the child process is created is known as the parent process.
child process make an exact copy of the parent process at the moment of fork.

* `fork`  

* `forksleep`

* `forkwait`

### Exec

* `forkexec`, `forkexec2`

    The execvp function is similar to execv , except that it searches the directories listed in the PATH environment variable (see Standard Environment Variables) to find the full file name of a file from filename if filename does not contain a slash.

* `execdemo`

### Parsing

* `strsep`

* `parse`

### Pipes

Pipes are useful for communication between related processes(inter-process communication).
Pipe is one-way communication only i.e we can use a pipe such that 
One process write to the pipe, and the other process reads from the pipe. 

If a process tries to read before something is written to the pipe, 
the process is suspended until something is written.
The pipe system call finds the first two available positions 
in the process’s open file table and allocates them for the read and write ends of the pipe.

* `pipe`
    - pipe-with-example
* `dup2`
    - dup2-with-example
* `pipeexec`
    - sharing-pipe-with-example


How is pipe in shell implemented?
    - cmd1 | cmd2
    - The stdout of cmd1 is sent to the stdin of cmd2

what is a file descriptor? 
```c
int fd = open(“filename”, MODE);
_ = read(fd, buf, count);
```
    
- Special fd:
• `stdin`: 0
• `stdout`: 1
• `stderr`: 2


`signals` with example



