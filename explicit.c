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

void *head = NULL;

/* myMethods */
// adds free block pointed by ptr to the free_list
static void free_list_push(void* newNode)
{
    /*setnextptr(newNode, head);
    setprevptr(head, newNode);
    setprevptr(newNode, NULL);
    head = newNode;*/
    
    /* since we are adding at the beginning,
       prev is always NULL */
    //new_node->prev = NULL;
    setprevptr(newNode, NULL);

    /* link the old list of the new node */
    //new_node->next = (*head_ref);
    setnextptr(newNode, head);

    /* change prev of head node to new node */
    //if ((*head_ref) != NULL)
    //(*head_ref)->prev = new_node;
    if(head !=NULL)
        setprevptr(head, newNode);
        

    /* move the head to point to the new node */
    //(*head_ref) = new_node;
    head = newNode;
    
}


// deletes free block pointed by ptr to the free_list
static void free_list_pop(void* ptr)
{
  
    if(getprevptr(ptr)==NULL)//if ptr points to root of free_list
            {
                head=getnextptr(ptr);
            }
    else
        {
            //head = getprevptr(bp);
            //setnextptr(bp, head);
            //setnextptr(bp, NEXT_BLKP (bp));
            //setnextptr(head , NEXT_BLKP (bp));
            
            //head = getprevptr(bp);
        
        //else//if ptr points to any arbitary block in free_list
        //NEXT_PTR(PREV_PTR(ptr))=NEXT_PTR(ptr);
            setnextptr(getprevptr(ptr), getnextptr(ptr));
        
        }
    
   setprevptr( getnextptr(ptr), getprevptr(ptr) );
}


static void *find_fit(size_t asize)//Iterate Down a Listn
{
    /* First-fit search */
    void *cur = head;

    //while(*(unsigned long*)NEXT_PTR (head) != 0)
    //{
    /*if ( (asize <= GET_SIZE(HDRP(head))))
            {
            //if(NEXT_PTR(bp) !=NULL)
            //{
            //segment_start = NEXT_PTR(bp);
                    //}
                return head;
                //break;
            }
        //segment_start= NEXT_BLKP(segment_start);
        //segment_start =  NEXT_BLKP(segment_start);
        
        //}
        else
            {*/
    //while(*(unsigned long*)NEXT_PTR (head) != 0)
                    while(cur != NULL)
                    {
                        //head =(unsigned long*) *(unsigned long*) NEXT_PTR(head);
                        //cur = NEXT_PTR(cur);
                        if ( (asize <= GET_SIZE(HDRP(cur))))
                            {
            //if(NEXT_PTR(bp) !=NULL)
            //{
            //segment_start = NEXT_PTR(bp);
                    //}
                                return cur;
                //break;
                            }
                        cur = getnextptr (cur); //NEXT_PTR(bp);<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
                    }
                //}
        
    return NULL; /* No fit */
    //#endif
}



static void *coalesce(void *newNode)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(newNode))) || PREV_BLKP(newNode) == newNode ; // prev_alloc will be true if previous block is allocated or its size is zero
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(newNode)));
    size_t size = GET_SIZE(HDRP(newNode));

    if (prev_alloc && next_alloc) { // Case 1
        // adds free block pointed by ptr to the free_list
        free_list_push(newNode);       
        return newNode;//<=====================================================================
    }

    else if (prev_alloc && !next_alloc)
        { // Case 2
        size += GET_SIZE(HDRP(NEXT_BLKP(newNode)));

        free_list_pop(NEXT_BLKP(newNode));//<===================================================================

        
        PUT(HDRP(newNode), PACK(size, 0));
        PUT(FTRP(newNode), PACK(size,0));

        /* base case */
        /*if (head == NULL)
            {
                return newNode;
            }*/

        /* If node to be deleted is head node */
        /*if (head == NEXT_BLKP(newNode) || NEXT_BLKP(newNode) == NULL)
            {
                setprevptr(newNode, NULL);
                setnextptr(newNode, getnextptr(NEXT_BLKP(newNode)));           
                //head = getnextptr(NEXT_BLKP(newNode));

                //setprevptr(newNode, getprevptr(NEXT_BLKP(newNode)));
                //setnextptr(newNode, getnextptr(NEXT_BLKP(newNode)));

                PUT(HDRP(newNode), PACK(size, 0));
                PUT(FTRP(newNode), PACK(size,0));
                
                head = newNode;

                return newNode;
                
            }*/
                 
        /*if(getprevptr(NEXT_BLKP(newNode)) == NULL && getnextptr(NEXT_BLKP(newNode)) != NULL  )//i.e., head
            {
                
                //setnextptr(getnextptr(NEXT_BLKP(newNode)), newNode);
                setnextptr(getnextptr(NEXT_BLKP(newNode)), newNode);
                //setnextptr(head, newNode);
                    
                setprevptr(newNode, NULL);
                setnextptr(newNode, getnextptr(NEXT_BLKP(newNode)));
                //head = newNode;
                PUT(HDRP(newNode), PACK(size, 0));
                PUT(FTRP(newNode), PACK(size,0));
                //head = newNode;
                free_list_push(newNode);    
                //return head;
                }*/
  
 
        /*if(getnextptr(NEXT_BLKP(newNode)) != NULL  &&  getprevptr(NEXT_BLKP(newNode)) != NULL)
            {
               setnextptr(getprevptr(NEXT_BLKP(newNode)), newNode);
               setprevptr(newNode, getprevptr(NEXT_BLKP(newNode)));
               setnextptr(newNode, getnextptr(NEXT_BLKP(newNode)));                           
               PUT(HDRP(newNode), PACK(size, 0));
               PUT(FTRP(newNode), PACK(size,0));        
               free_list_push(newNode);
            }*/

        /* Change next only if node to be deleted is NOT the last node */
        /*if ( getnextptr(NEXT_BLKP(newNode)) != NULL )
            {
                //del->next->prev = del->prev;
                setprevptr(getnextptr(NEXT_BLKP(newNode)), newNode);
            
                //setprevptr(newNode, getprevptr(NEXT_BLKP(newNode)));
                //setnextptr(newNode, getnextptr(NEXT_BLKP(newNode)));
            }*/
        
        
            /*if(getprevptr(NEXT_BLKP(newNode)) != NULL && getnextptr(NEXT_BLKP(newNode)) == NULL )//i.e., last node
            {
                setnextptr(getprevptr(NEXT_BLKP(newNode)), newNode);
                setprevptr(newNode, getprevptr(NEXT_BLKP(newNode)));
                setnextptr(newNode, NULL);
                PUT(HDRP(newNode), PACK(size, 0));
                PUT(FTRP(newNode), PACK(size,0));
                free_list_push(newNode);                
                }*/

        /* Change prev only if node to be deleted is NOT the first node */
        /*if (getprevptr(NEXT_BLKP(newNode))  != NULL )
            {
                //del->prev->next = del->next;
                setnextptr(getprevptr(NEXT_BLKP(newNode)), newNode);
                
                //setprevptr(newNode, getprevptr(NEXT_BLKP(newNode)));
                //setnextptr(newNode, getnextptr(NEXT_BLKP(newNode)));


                }*/

        //setprevptr(newNode, getprevptr(NEXT_BLKP(newNode)));
        //setnextptr(newNode, getnextptr(NEXT_BLKP(newNode)));

        //free_list_pop(NEXT_BLKP(newNode));

        //PUT(HDRP(newNode), PACK(size, 0));
        //PUT(FTRP(newNode), PACK(size,0));
        
        //free_list_push(newNode);//<====================================
        //return newNode;//<====================================                        
    }

    /*else if (!prev_alloc && next_alloc) { // Case 3
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    else { // Case 4
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
            GET_SIZE(FTRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        }*/


    free_list_push(newNode);//<=====================================================================  
    return newNode;
}

static void place(void *bp, size_t asize)
{
    size_t csize = GET_SIZE(HDRP(bp));

    if ((csize - asize) >= (2*DSIZE)) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        
        //free_list_delete(bp);// free block is deleted from free_list
        free_list_pop(bp);
        
        bp = NEXT_BLKP(bp);        
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
        coalesce(bp);
    }
    else {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
        //free_list_delete(bp);// free block is deleted from free_list
        free_list_pop(bp);
        
    }
    //headRef = bp;
    
}


bool myinit(void *heap_start, size_t heap_size)
{
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
    PUT((char *)segment_start + (1*DSIZE), PACK(2*DSIZE, 1)); /* Prologue header */
    PUT((char *)segment_start + (2*DSIZE), PACK(2*DSIZE, 1)); /* Prologue footer */

    head = (char *)segment_start + (4*DSIZE);
    segment_start = (char *)segment_start + (2*4*DSIZE);
    segment_size = segment_size - (2*4*DSIZE);

    /* Initialize free block header/footer and the epilogue header */
    //Figure 9.45 extend_heap extends the heap with a new free block
    PUT(HDRP(segment_start), PACK(segment_size, 0)); /* Free block header */
    PUT(FTRP(segment_start), PACK(segment_size, 0)); /* Free block footer */
    
    PUT(HDRP(NEXT_BLKP(segment_start)), PACK(0, 1)); /* New epilogue header */

    //size_t* prev = PREV_PTR(segment_start);l
    //size_t* next = NEXT_PTR(segment_start);

    //head = segment_start;
    coalesce(segment_start);
    

    
  return true;
}

void *mymalloc(size_t requested_size) {
  // TODO: remove the line below and implement this!

    size_t asize; /* Adjusted block size */
    char *bp;

    /* Ignore spurious requests */
    if (requested_size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (requested_size <= 4*DSIZE){
        asize = 8*DSIZE;
    }else{
        asize = roundup(requested_size, 4*ALIGNMENT);
    }

    /* Search the free list for a fit */
    if (((bp = find_fit(asize)) != NULL))
        {
            place(bp, asize);
            nused+=asize;//<-----------------
        //segment_size-=asize;//<--------------

            //if(*(unsigned long*) NEXT_PTR(head))
                //{
                    //head =(unsigned*) *(unsigned long*)NEXT_PTR(head);
                    
                    //}

            
            /*if(! *(unsigned long*) NEXT_PTR(bp))
                {
                    head =  NEXT_BLKP(head);
                    //return bp;
                   
                }
                else 
                    {
                        
                        head =(unsigned long*) *(unsigned long*) NEXT_PTR(head);
                        //  *(unsigned long*)PREV_PTR(head) = 0;
                    }*/
                //bp =  NEXT_PTR(bp);
       
    
            return bp;//
         }
    return bp;

   
}

void myfree(void *bp) {
  // TODO: implement this!
    if(bp !=NULL)
        {
            size_t size = GET_SIZE(HDRP(bp));

            PUT(HDRP(bp), PACK(size, 0));
            PUT(FTRP(bp), PACK(size, 0));
            nused-=size;

            //free_list_push(bp);
            coalesce(bp);
            

        //head = HDRP (bp);
        //unsigned long HDRP_head = (unsigned long*)HDRP (head);
        /*
        *(unsigned long*)bp = (unsigned long) (unsigned long*) (head);
        //unsigned long HDRP_bp = (unsigned long*)HDRP (bp);
        *(unsigned long*)PREV_PTR (head) = (unsigned long) (unsigned long*) (bp);
        *(unsigned long*)PREV_PTR (bp) = 0;
        
        head = (bp);
        //head = (unsigned long*) (unsigned long) *(unsigned long*) PREV_PTR(head);
        
       
        //coalesce(bp);//<-------------------
        //head = (bp);}*/
        }
     

}

void *myrealloc(void *old_ptr, size_t new_size) {
  // TODO: remove the line below and implement this!
  return NULL;
}

bool validate_heap()
{
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
