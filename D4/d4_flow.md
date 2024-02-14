# Discussion 04

## `wait`

// wait
- `wait.c`

// wait options
- `waithang.c`
- `waituntraced.c`


## Process group

Every process belongs to a process group. They are generally a collection of
    processes associated with the same job/ terminal.

Each process group has a unique process group id (pgid) (positive integers).

Each process group has a process group leader.
    How to identify the leader?
    The leader's process ID (pid) is the same as its process group ID.


- `pgrp.c`
- `wait2child.c`
- `wait2child.v2.c`
