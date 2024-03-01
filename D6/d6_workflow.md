# Discussion 06

`mmap` -- allocate memory, or map files or devices into memory

mmap() is a system call that can be used by a user process to ask the operating system kernel to map either files or devices into the memory (i.e., address space) of that process. The mmap() system call can also be used to allocate memory (an anonymous mapping). 
[REF](https://www.clear.rice.edu/comp321/html/laboratories/lab10/)

```c
#include <sys/mman.h>

void *
mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
```

## malloc using `mmap`

1. `mmap-malloc.c`
This example creates an array of N integers using mmap. 
The mmap can be similar to malloc when used with the 
MAP_ANONYMOUS and MAP_PRIVATE flags.

## file-backed mapping using `mmap` 
[filebacked-mmap.png]

2. `mmap-fileread.c`
This example 
    a) maps a file using mmap,
    b) and reads the file through the mmapped memory.

## mmap flags

### MAP_FIXED

3. `mmap-fixed.c`
This example creates an array of N integers at a fixed address 
using mmap with MAP_FIXED.

### MAP_PRIVATE

When we use MAP_PRIVATE, changes to mmapped file are NOT reflected in the original file.

4. `mmap-private.c`
This example 
    a) mmaps a file in MAP_PRIVATE mode,
    b) and writes to the mmapped file. 
But, the write is NOT reflected in the original file.

### MAP_SHARED

When we use MAP_SHARED, changes to mmapped file are reflected in the original file.

5. `mmap-shared.c`
This example 
    a) mmaps a file in MAP_SHARED mode,
    b) and writes to the mmapped file. 
The write is reflected in the original file.

### MAP_GROWSUP

Does not exist in original `mmap.

### forking and MAP_ANON

6. `mmap-fork-anon-private.c`
This example 
    a) creates an array of N integers in the parent using mmap with MAP_PRIVATE flag
        ( 0, 10, 20, ....),
    b) and changes the first element in the child process
        (99, 10, 20, ...).
But, the parent can NOT see the change because of the MAP_PRIVATE flag.

6. `mmap-fork-anon-shared.c`
This example 
    a) creates an array of N integers in the parent using mmap with MAP_SHARED flag
        ( 0, 10, 20, ....),
    b) and changes the first element in the child process
        (99, 10, 20, ...).
The parent CAN see the change because of the MAP_SHARED flag.

### forking and file-backed mapping

7. `mmap-fork-private.c`
This example
 a) mmaps a file in MAP_PRIVATE mode in the parent,
 b) writes to the mmapped memory in the child,
 c) reads the mmapped memory in the parent.

The write by child is NOT reflected in the parent because of MAP_PRIVATE.

8. `mmap-fork-shared.c`
This example
 a) mmaps a file in MAP_SHARED mode in the parent,
 b) writes to the mmapped memory in the child,
 c) and reads the mmapped memory in the parent.
The write by child is NOT reflected in the parent because of MAP_PRIVATE.

## Implementing `mmap`

### xv6 memory management

Physical memory refers to storage cells in DRAM. A byte of physical memory has an address, called a physical address. Instructions use only virtual addresses, which the paging hardware translates to physical addresses, and then sends to the DRAM hardware to read or write storage.

#### `memlayout.h` 
[xv6-memlayout.png]

A process’s user memory starts at virtual address zero and can grow up to `KERNBASE`, allowing a process to address up to 2 gigabytes of memory. 

```c
// Memory layout
...
#define PHYSTOP 0xE000000           // Top physical memory
...
#define KERNBASE 0x80000000         // First kernel virtual address
...
// You might consider storing 0x60000000 here
#define V2P(a) (((uint) (a)) - KERNBASE)
#define P2V(a) ((void *)(((char *) (a)) + KERNBASE))
...
```


#### `mmu.h`
[x86-pagetable-hw.png]

Each process has a separate page table, and xv6 tells the page table hardware to switch page tables when xv6 switches between processes. 

When a process asks xv6 for more memory, xv6 first finds free physical pages to
provide the storage, and then adds PTEs to the process’s page table that point to the
new physical pages. xv6 sets the PTE_U, PTE_W, and PTE_P flags in these PTEs. 

```c
#define PGSIZE          4096    // bytes mapped by a page
...
#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))
...
// Page table/directory entry flags.
#define PTE_P           0x001   // Present
#define PTE_W           0x002   // Writeable
#define PTE_U           0x004   // User

// Address in page table or page directory entry
#define PTE_ADDR(pte)   ((uint)(pte) & ~0xFFF) // get the first 20 bits
#define PTE_FLAGS(pte)  ((uint)(pte) &  0xFFF) // get the last 12 bits
...
```

* `PTE_P` indicates whether the PTE is present; if it is not present, a reference to the page causes a fault. It arises the `case T_PGFLT:` (14) in `trap.c`
* `PTE_W` controls whether instructions are allowed to issue writes to the page; if not set, only reads and instruction fetches are allowed.
* `PTE_U` controls whether user programs are allowed to use the page; if clear, only the kernel is allowed to use the page.


#### `kalloc.c`
xv6 allocates and frees whole 4096-byte pages at a time.
- `kalloc()`
// Allocate one 4096-byte page of physical memory. Returns a pointer that the kernel can use. Returns 0 if the memory cannot be allocated.

- `kfree()`
// Free the page of physical memory pointed at by v, which normally should have been returned by a call to kalloc().  

#### `vm.c`

- `mappages()`
// Create PTEs for virtual addresses starting at va that refer to physical addresses starting at pa. va and size might not be page-aligned.
- `walkpgdir()`
// Return the address of the PTE in page table pgdir that corresponds to virtual address va.  If alloc!=0, create any required page table pages.

#### creating an address space

`main.c -> main() ->  kvmalloc()`
    create and switch to a page table with the mappings above KERNBASE required for the kernel to run.
`mappages()` in `vm.c`

### random things
- purpose of `defs.h`
- you may need some data structure to store information related to each mapping, which may include things like the mapped addr, len, prot, flags etc. 

### Support file-backed mapping: read or write file
`file.c`
- fileread()
- filewrite()

modify `fork()` and `exit()`
- copy mappings from parent to child across the fork() system call. 
- make sure you remove all mappings in exit().

### Support MAP_GROWSUP: handle page fault

`trap.c`
```c
  case T_PGFLT: //14  
    cprintf("\tPagefault at virtual addr %x\n", rcr2());
```

### Best practices (Optional for P5) 

#### lazy allocation
The mapped pages are not actually brought into physical memory until they are referenced; thus mmap() can be used to implement lazy loading of pages into memory (demand paging). 

```c
  case T_PGFLT: //14  
    cprintf("\tPagefault at virtual addr %x\n", rcr2());
```


