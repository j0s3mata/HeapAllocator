#include "allocator.h"
#include "debug_break.h"

#include "segment.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//private (static) global variables 
static void *segment_start= NULL;
static size_t segment_size = 0;
static size_t nused;

//static void *next_avail, *heap_max;

static void *find_fit(size_t asize)
{
/* First-fit search */
   void *bp;
    
   for (bp = segment_start; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
       if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
           return bp;
       }
         }
   return NULL; /* No fit */
   //#endif
}

static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));
    
        if ((csize - asize) >= (2*DSIZE)) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
         }
    else {
         PUT(HDRP(bp), PACK(csize, 1));
         PUT(FTRP(bp), PACK(csize, 1));
         }
}


 
bool myinit(void *heap_start, size_t heap_size) {
   /* TODO: remove the line below and implement this!
   * This must be called by a client before making any allocation
   * requests.  The function returns true if initialization was
   * successful, or false otherwise. The myinit function can be
   * called to reset the heap to an empty state. When running
   * against a set of of test scripts, our test harness calls
   * myinit before starting each new script.
   */

    segment_start = heap_start;
    segment_size = heap_size;
    nused = 0;
 
     /* Create the initial empty heap */
    if (segment_start == NULL) return false;
         
    PUT((char *)segment_start, 0);                          /* Alignment padding */
    PUT((char *)segment_start + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */
    PUT((char *)segment_start + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
    PUT((char *)segment_start + (3*WSIZE), PACK(0, 1));     /* Epilogue header */
    segment_start = (char *)segment_start + (4*WSIZE);
    segment_size = segment_size - (4*WSIZE);

     /* Initialize free block header/footer and the epilogue header */
    //Figure 9.45 extend_heap extends the heap with a new free block
    PUT(HDRP(segment_start), PACK(segment_size, 0)); /* Free block header */
    PUT(FTRP(segment_start), PACK(segment_size, 0)); /* Free block footer */
    PUT(HDRP(NEXT_BLKP(segment_start)), PACK(0, 1)); /* New epilogue header */
    
 
   return true;
}

void *mymalloc(size_t requested_size) {
    // TODO: remove the line below and implement this!
    size_t asize; /* Adjusted block size */
    //size_t extendsize; /* Amount to extend heap if no fit */
    char *bp;
    
         /* Ignore spurious requests */
         if (requested_size == 0)
         return NULL;
    
         /* Adjust block size to include overhead and alignment reqs. */
         if (requested_size <= 2*DSIZE){
         asize = 4*DSIZE;
         }else{
             asize = roundup(requested_size, 2*ALIGNMENT);
         }
    
         /* Search the free list for a fit */
         if ((bp = find_fit(asize)) != NULL) {
         place(bp, asize);
         nused+=asize;//<-----------------
         //segment_size-=asize;//<--------------
         return bp;
         }

       
    
         /* No fit found. Get more memory and place the block */
         //extendsize = MAX(asize,CHUNKSIZE);
         //if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
         //return NULL;
         //place(bp, asize);
         return bp;
          
}

void myfree(void *bp) {
    // TODO: implement this!
    size_t size = GET_SIZE(HDRP(bp));
   
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    nused-=size;
    //coalesce(bp);
}

void *myrealloc(void *bp, size_t nsize) {
    // TODO: remove the line below and implement this!
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t next_size = GET_SIZE(HDRP(NEXT_BLKP(bp)));
    
    size_t csize = GET_SIZE(HDRP(bp));
    

    if(nsize <= csize)
        {
            place(bp, nsize);  
        }
    else if ((nsize <= (csize + 2*DSIZE +  next_size + DSIZE)) && !next_alloc)
        { /* Case 2 */
        csize+= 2*DSIZE +  next_size + DSIZE;
        nsize = roundup(nsize, 2*ALIGNMENT);

        if ((csize - nsize) >= (2*DSIZE))
            {
            PUT(HDRP(bp), PACK(nsize, 1));
            PUT(FTRP(bp), PACK(nsize, 1));
            nused+=nsize;
            bp = NEXT_BLKP(bp);
            PUT(HDRP(bp), PACK(csize-nsize - DSIZE, 0));
            PUT(FTRP(bp), PACK(csize-nsize - DSIZE, 0));
            }
        else
            {
            PUT(HDRP(bp), PACK(csize, 1));
            PUT(FTRP(bp), PACK(csize, 1));
            nused+=csize;
            }       
        }
    else if (nsize > csize)
        {
            bp= mymalloc(nsize);
            if (!bp) return NULL;
                    
        } 
  return bp;
}

bool validate_heap() {
  /* TODO: remove the line below and implement this to
   * check your internal structures!
   * Return true if all is ok, or false otherwise.
   * This function is called periodically by the test
   * harness to check the state of the heap allocator.
   * You can also use the breakpoint() function to stop
   * in the debugger - e.g. if (something_is_wrong) breakpoint();
   */

  return true;
}

/* Function: dump_heap
 * -------------------
 * This optional function dumps the raw heap contents.
 * This function is not called from anywhere, it is just here to
 * demonstrate how such a function might be a useful debugging aid.
 * You are not required to implement such a function in your own
 * allocators, but if you do, you can then call the function
 * from gdb to view the contents of the heap segment.
 * For the bump allocator, the heap contains no block headers or
 * heap housekeeping to provide structure, so all that can be displayed
 * is a dump of the raw bytes. For a more structured allocator, you
 * could implement dump_heap to instead just print the block headers,
 *GET_SIZE(HDRP(segment_start)) & GET_ALLOC(HDRP(segment_start))
 * which would be less overwhelming to wade through for debugging.
 */
void dump_heap() {
    printf("Heap segment starts at address %p, ends at %p. %lu bytes currently "
           "used.",
           segment_start, (char *)segment_start + segment_size, nused);
    for (int i = 0; i < nused; i++) {
        unsigned char *cur = (unsigned char *)segment_start + i;
        if (i % 32 == 0) {
            printf("\n%p: ", cur);
        }
        printf("%02x ", *cur);
    }
}

void heap_dump(){
void *bp;
    
 for (bp = segment_start; (char*) bp < (char *)segment_start + segment_size ; bp = NEXT_BLKP(bp)) {

     size_t size = GET_SIZE(HDRP(bp));
     size_t alloc = GET_ALLOC(HDRP(bp));
         
     printf("%lu / %lu\n" , alloc, size);
 }


}
