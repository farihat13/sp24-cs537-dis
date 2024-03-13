# Discussion 08

[P5](https://git.doit.wisc.edu/cdis/cs/courses/csx537/spring24/public/p5/-/blob/main/instructions/instructions.md?ref_type=heads)


## Sleeplock

in spinlock.h
```c
// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?
  // ...
};
```

in sleeplock.h
```c
// Long-term locks for processes
struct sleeplock {
  uint locked;       // Is the lock held?
  struct spinlock lk; // spinlock protecting this sleep lock
  // ...
};
```

sleeplock.c
```c
initsleeplock(struct sleeplock *lk, char *name);
acquiresleep(struct sleeplock *lk);
releasesleep(struct sleeplock *lk);
```

It is different from a regular spinlock because it allows a process to sleep while waiting for the lock to become available, rather than continuously spinning and wasting CPU cycles.

In a system using x86 architecture, the instructions CLI (Clear Interrupt) and STI (Set Interrupt).

### Acquire the lock


inside spinlock `void acquire(struct spinlock *lk)`
```c
  // The xchg is atomic.
  while(xchg(&lk->locked, 1) != 0)
    ;
```
`xchg(&lk->locked, 1)` atomically sets `lk->locked` to 1 and returns the old value of lk->locked. This operation is performed in a single atomic step to prevent race conditions.

inside sleeplock.c
```c
void acquiresleep(struct sleeplock *lk) {
    // This spinlock (lk->lk) protects the sleep lock's internal state (lk->locked and lk->pid) 
    // from concurrent access.
    acquire(&lk->lk); 
    // check if the sleeplock is already locked by any other process or not
    while (lk->locked) { 
        // if the sleep lock is currently locked (lk->locked is 1), 
        // the process needs to wait for the lock to become available.
        sleep(lk, &lk->lk); 
    }
    // the current process has acquired the lock
    lk->locked = 1; 
    lk->pid = myproc()->pid; 
    // releases the spinlock (lk->lk). Now, other processes can attempt to acquire the sleeplock.
    release(&lk->lk); 
}
```

in proc.c
```c
// Atomically release lock and sleep on chan. Reacquires lock when awakened.
void sleep(void *chan, struct spinlock *lk) {
    struct proc *p = myproc();
    // ...
    // Must acquire ptable.lock in order to change p->state and then call sched.
    // Once we hold ptable.lock, we can be guaranteed that we won't miss any wakeup
    // (wakeup runs with ptable.lock locked), so it's okay to release lk.
    if(lk != &ptable.lock){  //DOC: sleeplock0
        acquire(&ptable.lock);  //DOC: sleeplock1
        release(lk);
    }
    // Go to sleep.
    p->chan = chan;
    p->state = SLEEPING;
    // schedules another process to run. the current process will remain asleep until 
    // it is explicitly woken up by a wakeup call on the same channel.
    sched(); // TODO: 
    // Tidy up.
    p->chan = 0;
    // Reacquire original lock.
    if(lk != &ptable.lock){  //DOC: sleeplock2
        release(&ptable.lock);
        acquire(lk);
    }
}
```
The "channel" is a way to identify the condition or resource that the process is waiting for. When a process sleeps on a channel, it is essentially saying, "I am waiting for something specific to happen, and until it does, I don't need to use the CPU."


### Release the lock

inside spinlock.c
```c
  asm volatile("movl $0, %0" : "+m" (lk->locked) : );
```

inside sleeplock.c
```c
void releasesleep(struct sleeplock *lk) {   
    // acquire the spinlock
    acquire(&lk->lk); 
    // assuming the current process holds the lock, so no checking in loop
    lk->locked = 0;
    lk->pid = 0;
    // wakes up any processes that may be sleeping on this channel, waiting for 
    // the lock to become available
    wakeup(lk);
    // release the spinlock
    release(&lk->lk);
}
```


in proc.c
```c
// Wake up all processes sleeping on chan. The ptable lock must be held.
static void wakeup1(void *chan) {
  struct proc *p;
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan)
      p->state = RUNNABLE;
}
// Wake up all processes sleeping on chan.
void wakeup(void *chan) {
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}
```

## Scheduler

in main.c

```c
  scheduler();     // start running processes
```

in proc.c

```c
void scheduler(void) {
    // ...
    for(;;){ // infinite loop
        // ...
    }
}
```

in proc.h
```c
enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
// Per-process state
struct proc {
    // ...
    char *kstack;                // Bottom of kernel stack for this process
    enum procstate state;        // Process state
    int pid;                     // Process ID
    struct proc *parent;         // Parent process
    struct trapframe *tf;        // Trap frame for current syscall
    struct context *context;     // swtch() here to run process
    // ...
};
```

in proc.c
```c
// Inside the scheduler infinite loop; Loop over process table looking for process to run.
acquire(&ptable.lock);
for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state != RUNNABLE) continue;
    // Switch to chosen process. It is the process's job to release ptable.lock 
    // and then reacquire it before jumping back to us.
    c->proc = p;
    switchuvm(p); // Switch TSS (task state segment) and h/w pgtable to correspond to process p
    p->state = RUNNING; // change from RUNNABLE to RUNNING
    
    // currently running on scheduler's own kernel stack
    // swtch is going to save current register values in c->scheduler, and then
    // is going to load the p->context in the register
    //      1. save old callee-saved registers in scheduler's stack; They will be part of the old process's context.
    //      2. store the current stack pointer in the old process context
    //      3. loads the stack pointer of the new process; This switches the stack to the new process's kernel stack.
    //      4. load the callee-saved registers from the new proces stack
    //      5. At this point, the new process resumes execution; it might be returning from
    //         trap (after the yield() line) and start executing the usercode
    swtch(&(c->scheduler), p->context);
    switchkvm(); // switch to the kernel page table
    // Process is done running for now. It should have changed its p->state before coming back.
    c->proc = 0;
}
release(&ptable.lock);
```

in trap.c
```c
if(myproc() && myproc()->state == RUNNING && tf->trapno == T_IRQ0+IRQ_TIMER)
    // yield the CPU; change state from RUNNING to RUNNABLE; swtch kernel stack 
    // and return into the scheduler
    yield(); // myproc()->state = RUNNABLE; sched();
// Resumes here after swtch in scheduler
```

GDB
`break proc.c:350 if p->pid > 3`

`testloop &; testloop &`