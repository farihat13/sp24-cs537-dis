# Discussion 04

// wait
- `wait.c`

// Status analysis macros:
- `waitdetails.c`

// wait options
- `waithang.c`
- `waituntraced.c`

// signals
- `signals.c`

// signalchld
- `signalchld.c`


## Process group

resources:
- https://youtu.be/NfHqGv0PlIw
- https://www.baeldung.com/linux/posix
- https://unix.stackexchange.com/questions/404555/
- what-is-the-purpose-of-the-controlling-terminal



Every process belongs to a process group. They are generally a collection of
    processes associated with the same job/ terminal.

Each process group has a unique process group id (pgid) (positive integers).

Each process group has a process group leader.
    How to identify the leader?
    The leader's process ID (pid) is the same as its process group ID.


POSIX stands for Portable Operating System Interface.  It defines a set of rules 
and standards for how processes are created, managed, and interact with each other 
and the operating system.
So, POSIX provides a standardized way to work with processes on Unix-like systems.

POSIX introduces the concept of process groups and sessions, allowing processes 
to be organized into hierarchies and controlled collectively.

### POSIX Base Definitions

#### Process Group
A collection of processes that permits the signaling of related processes. 
    Each process in the system is a member of a process group that is 
    identified by a process group ID. 
    A newly created process joins the process group of its creator.

#### Process Group ID

#### Process Group Leader
A process whose process ID is the same as its process group ID.     

#### Process Group Lifetime
The period of time that begins when a process group is created and 
    ends when the last remaining process in the group leaves the group, 
    due either to the end of the lifetime of the last process or to the 
    last remaining process calling the setsid() or setpgid() functions.

#### Session
A collection of process groups established for job control purposes. 
    Each process group is a member of a session. A process is considered 
    to be a member of the session of which its process group is a member. 
    A newly created process joins the session of its creator. 
    A process can alter its session membership; see setsid(). 
    There can be multiple process groups in the same session.

#### Session Leader
A process that has created a session.

#### Session Lifetime
The period between when a session is created and the end of the lifetime 
    of all the process groups that remain as members of the session.

#### Job Control
Usually a job means a pipeline of processes.  

This permits a shell user to simultaneously execute multiple 
    commands (or jobs), one in the foreground and all remaining 
    in the background. It is also possible to move the jobs from 
    the foreground to the background and vice-versa.

### Terminal control

#### Controlling Terminal
The controlling terminal is the terminal that the session leader 
    (and subsequently, the processes within that session) is connected to.
    It is responsible for handling input and output for the processes 
    running in the foreground.

A controlling terminal is a terminal where a Linux process starts. 
    For example, if you run a command from the shell, the terminal window 
    where you entered the command is the controlling terminal.
    Processes launched with controlling terminals inherit attributes of 
    the parent terminal. 

It’s easy to see processes that have a controlling terminal in Linux.
    You can use the ps command to see them.

$ `ps`            # shows the processes that have a controlling terminal
                    # TTY indicates a process’s controlling terminal

        PID     TTY          TIME   CMD
    2260131     pts/4    00:00:00   bash
    2556344     pts/4    00:00:00   ps

$ `ps aux`        # shows all processes whether they have a controlling terminal or not

    USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
    root           1  0.0  0.0 168112 13348 ?        Ss   Oct02   1:00 /sbin/init
    root           2  0.0  0.0      0     0 ?        S    Oct02   0:00 [kthreadd]
    root           3  0.0  0.0      0     0 ?        I<   Oct02   0:00 [rcu_gp]
    ...

A question mark (?) means the process does not have a controlling terminal. 

Q. Why does this happen?
    
Linux systems run a lot of daemons, or services, in the background. 
        Such as running your network connection. These don’t need to use a terminal, 
        so they just run without one.

Q. What is the purpose of the controlling terminal?
    
With controlling terminal, process is able to tell kernel which 
        process group is foreground process group (in same session). 
        If there is a foreground process group for a terminal, 
        we can control the foreground process group via the terminal, 
        e.g., Ctrl-C/Ctrl-\ to terminate the foreground process group. 

With controlling terminal, even if you already redirect stdin to 
        other places/files, you still be able to read/write from/to 
        controlling terminal, the /dev/tty. This special file is a synonym 
        within the kernel for the controlling terminal of current process.

The controlling terminal can send terminal control signals 
        (e.g., SIGINT, SIGTSTP) to the processes running in the foreground.

$ `proc1 | proc2 &`

$ `proc3 | proc4 | proc5 &`
    
$ `ps -o pid,ppid,pgid,sid,comm`

        PID  PPID     PGID     SID  COMMAND  TT
    1772652 2533528 1772652 2533528 proc3    pts/4          # pgrp leader of job `proc3 | proc4 | proc5 &`
    1772655 2533528 1772652 2533528 proc4    pts/4
    1772656 2533528 1772652 2533528 proc5    pts/4
    1795937 2533528 1795937 2533528 ps       pts/4
    2533528 3062708 2533528 2533528 bash     pts/4          # session leader
    3413450 2533528 3413450 2533528 proc1    pts/4          # pgrp leader of job `proc1 | proc2 &`
    3413451 2533528 3413450 2533528 proc2    pts/4


    
```C
#include <sys/types.h>
#include <unistd.h>

int tcsetpgrp(int fd, pid_t pgid);
```

`tcsetpgrp` sets the process group identification (PGID) for the 
foreground process group associated with a controlling terminal. 
The arguments to tcsetpgrp are:

- `fd`
    a file discriptor that refers to a terminal device. 
    The file descriptor must be for the controlling terminal 
    associated with the process calling tcsetpgrp.
- `pgid`
    a foreground PGID (process group identification number) 
    from the same session as the process calling tcsetpgrp.

The `tcgetpgrp` function can be called from a background process 
to obtain the PGID of a foreground process group. 
The process can then change from the background group 
to the foreground process group by making a call to tcsetpgrp and 
specifying the PGID of the new foreground group as one of the arguments 
passed to tcsetpgrp.
