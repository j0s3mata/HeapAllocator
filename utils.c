#include "utils.h"

#define SIZE_T 8


inline size_t MAX(size_t x, size_t y)
{
    return ((x) > (y)? (x) : (y));
}

/* Pack a size and allocated bit into a word */
inline size_t PACK(size_t size, size_t alloc)
{
    return  ((size) | (alloc));//warning: unused variable ‘boundary_tag’
}                                                             // line:vm:mm:pack

/* Read and write a word at address p */
inline size_t GET(void *p)                                           // line:vm:mm:get
{
    return  (*(unsigned int *)(p));
}

inline void PUT(void *p, size_t val)                                 // line:vm:mm:put
{
    (*(unsigned int *)(p) = (val));
}

/* Read the size and allocated fields from address p */
inline size_t GET_SIZE(void *p)
{
    return (GET(p) & ~0x7); // line:vm:mm:getsize
}

inline size_t GET_ALLOC(void *p)
{
    return (GET(p) & 0x1); // line:vm:mm:getalloc
}

//The remaining macros operate on block pointers (denoted bp) that point to the first payload byte.
/* Given block ptr bp, compute address of its header and footer */
inline void* HDRP(void *bp)
{
    return ((char *)(bp)-2*WSIZE);// line:vm:mm:hdrp
}

inline void* FTRP(void *bp)
{
    return ((char *)(bp) + GET_SIZE(HDRP(bp)) - 2*DSIZE); // line:vm:mm:ftrp
}

/* Given block ptr bp, compute address of next and previous blocks */
inline void* NEXT_BLKP(void *bp)
{
    return ((char *)(bp) + GET_SIZE(((char *)(bp)-2*WSIZE))); // line:vm:mm:nextblkp
}


inline void* PREV_BLKP(void *bp)
{
    return ((char *)(bp)-GET_SIZE(((char *)(bp)-2*DSIZE))); // line:vm:mm:prevblkp
}

/* $end mallocmacros */

/* Function: roundup
 * -----------------
 * This function rounds up the given number to the given multiple, which
 * must be a power of 2, and returns the result.
 */
 size_t roundup(size_t sz, size_t mult) { return (sz + (mult) + mult - 1) & ~(mult - 1); }


/* Given block ptr bp, compute address of next and previous pointers */

/*void* PREV_PTR(void *bp)
{
    return *(char **)((char*)bp+2*WSIZE); // line:vm:mm:nextblkp
    //return (*(char **) (bp + (2*WSIZE) ) );
}

void* NEXT_PTR(void *bp)
{
    return (*(char **)(bp)); // line:vm:mm:prevblkp
}*/

inline char *getprevptr(char *ptr)
{
    return (char *)*(size_t*)(ptr);
}
inline void setprevptr(char *ptr, char *prev)
{
    //ptr += SIZE_T;
    *(size_t*)ptr = (size_t)prev;
}

inline char *getnextptr(char *ptr)
{
    return (char *)*(size_t*)(ptr + SIZE_T);;
}

inline void setnextptr(char *ptr, char *next)
{
    ptr += SIZE_T;
    *(size_t*)ptr = (size_t)next;
}
