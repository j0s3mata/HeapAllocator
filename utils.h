/* File: allocator.h
 * -----------------
 * Interface file for the custom heap allocator.
 */
#ifndef _UTILS_H
#define _UTILS_H

#include <stddef.h>



//Figure 9.43 Basic constants and macros for accessing and traversing the free list.
/* Basic constants and macros */
#define WSIZE 4 /* Word and header/footer size (bytes) */
#define DSIZE 8 /* Double word size (bytes) */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */

size_t MAX(size_t x, size_t y);

/* Pack a size and allocated bit into a word */
size_t PACK(size_t size, size_t alloc);  // line:vm:mm:pack

/* Read and write a word at address p */
size_t GET(void *p);                                           // line:vm:mm:get


void PUT(void *p, size_t val);                                 // line:vm:mm:put

/* Read the size and allocated fields from address p */
size_t GET_SIZE(void *p);                                     // line:vm:mm:getsize


size_t GET_ALLOC(void *p);                                    // line:vm:mm:getalloc


//The remaining macros operate on block pointers (denoted bp) that point to the first payload byte.
/* Given block ptr bp, compute address of its header and footer */
void* HDRP(void *bp);                                         // line:vm:mm:hdrp


void* FTRP(void *bp);                                         // line:vm:mm:ftrp


/* Given block ptr bp, compute address of next and previous blocks */
void* NEXT_BLKP(void *bp);                                    // line:vm:mm:nextblkp


void* PREV_BLKP(void *bp);                                    // line:vm:mm:prevblkp

/* $end mallocmacros */

/* Function: roundup
 * -----------------
 * This function rounds up the given number to the given multiple, which
 * must be a power of 2, and returns the result.
 */
size_t roundup(size_t, size_t);

/*void* NEXT_PTR(void *bp);

void* PREV_PTR(void *bp);*/

char *getprevptr(char *ptr);
void setprevptr(char *ptr, char *prev);
char *getnextptr(char *ptr);
void setnextptr(char *ptr, char *next);



#endif
