# Translate VA to PA from Memory Dump

## Given

1. Page size = 32 Bytes (2<sup>5</sup>) 

    This implies, 
        
        offset                           5 bits

2. #virtual pages = 1024 (2<sup>10</sup>)

    This implies, 
    
        VPN                             10 bits
        So, Virtual address is (10+5) = 15 bits

3. #physical pages = 128 (2<sup>7</sup>) 

    This implies, 

        PFN                              7 bits
        So, Physical address is (7+5) = 12 bits

4. The system assumes a multi-level page table. 
    - the upper five bits of a virtual address are used to index into a page directory; 
    - the page directory entry (PDE), if valid, points to a page of the page table. 
    - Each page table page holds 32 page-table entries (PTEs). (**optional**)
    - Each PTE, if valid, holds the desired translation (physical frame number, or PFN) of the virtual page in question.

    This implies,

        Virtual Address
        |  14 - 10 |  9 - 5     |  4 - 0   |
        |----------|------------|----------|   15 bits
        | PD Index |  PT Index  |  Offset  |

        #PageDir = 1
        #PDE = 2^5 = 32 (5 coming from PD Index)
        PDE
        |   8   |    6 - 0      |
        |-------| --------------|   8 bits = 1 byte
        | Valid | PFN 6 - PFN 0 |

        #Pagetable = #PDE = 2^5 = 32 (5 coming from PD Index)
        #PTE per pagetable = 2^5 = 32 (5 coming from PT Index)
        PTE
        |   8   |    6 - 0      |
        |-------| --------------|   8 bits = 1 byte
        | Valid | PFN 6 - PFN 0 |

5. PDBR = 18 (decimal)


## Observe Memory Dump

- 128 pages = #physical pages
- Each page has 32 bytes


## Useful formulas

```c
// From OSTEP
PDEAddr = PageDirBase        + (PDIndex * sizeof(PDE))
PTEAddr = (PDE.PFN << SHIFT) + (PTIndex * sizeof(PTE))
```

## Steps to translate VA to PA

0. Break down VA

1. Page Directory
    - PDE index
    - PDE contents

2. Page table
    - PTE index
    - PTE contents

3. Physical Address
    - PA
    - contents

## Translate VA = 0x3a3a to PA

0. Break down VA

    ```
    VA = 0x 3a3a = 0b 0011 1010 0011 1010
    PD Index = 0b 01110 = 14 (decimal)
    PT Index = 0b 10001 = 17 (decimal)
    Offset   = 0b 11010 = 26 (decimal)
    ```

1. Page Directory
    Since `PDBR = 18 (decimal)`, 
        the pagedir is in physical page 18.
    ```
    page  18: 7f c0 ea f9 ed 8b db ba d9 c1 84 8a b3 7f da eb 9a 85 ab 87 e5 97 b1 df 86 ec e7 ad f2 b9 d5 f8
    ```
    - PDE index: `14 (decimal)`

        So, the PDE is in 14th byte of physical page 18,
            which contains `da`.

    - PDE contents: `0x da`     
        ```
        0x da = 1101 1010 = 1  1011010
        Valid = 1
        PFN   = 1011010 = 90 (decimal)
        ```

2. Page table
    Since in PDE contains `PFN = 90 (decimal)`, 
        the pagetabe is in physical page 90.
    ```
    page  90: 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f e2 7f 7f 7f 7f 7f c7 7f 7f 7f 7f 7f 7f 7f 7f  
    ```
    - PTE index: `17 decimal`

        So, the PTE is in 17th byte of physical page 90,
            which contains `e2`.
            
    - PTE contents: `0x e2`
        ```
        0x e2 = 1110 0010 = 1 1100010
        Valid = 1
        PFN   = 1100010 = 98 (decimal)
        ```

3. Physical Address `PFN + Offset`
    ```
    0b 1100010 11010 = 0b 1100 0101 1010 = 0x c5a
    ```
    - PA: `0x c5a` 

        Since, in PA, `PFN = 98 (decimal)`, 
            the actual content is in physical page 90.
        
        ```
        page  98: 16 0d 18 10 02 0e 01 1c 1d 0a 09 17 06 05 05 0a 13 1d 06 1d 11 1b 19 04 14 03 00 0c 17 11 05 1a  
        ```

        Since `Offset = 26 (decimal)`,
            the actual content is in 26th byte of physical page 98,
            which contains `00`

    - contents: `0x 00`



