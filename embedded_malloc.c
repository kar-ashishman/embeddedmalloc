/*
    * malloc.c
    * Simple embedded memory allocator
    * Author: Ashishman kar
    * Date: 1st September 2025
    * Arguments: 
    *   size - size of memory block to be allocated in bytes
    *   alloc_method - allocation method, can be QUICKEST or BESTFIT
    * Returns: pointer to the allocated memory block, or NULL if allocation fails
    * Description: This file contains the implementation of a simple memory allocator for embedded systems.
    * 
*/


#include "malloc.h"
#include "memalloc.h"


void embedded_malloc_init() {
    elem_matrix[0].mem = &heap[0];
    elem_matrix[0].state = FREE;
    elem_matrix[0].size = HEAPSIZE;
}


unsigned char *embedded_malloc(unsigned short size, unsigned char alloc_method) {

    int balance_size = 0;
    int temp, i;

    // Can not allocate more than MAX_ELEM_SIZE blocks
    if (allocated_count > MAX_ELEM_SIZE) return NULL;

    unsigned char* result = NULL;
    int bestmatchidx = -1;
    int bestmatchsize = HEAPSIZE;

    for(i=0; i<2*MAX_ELEM_SIZE; i++) {
        // break if reached last valid block entry
        if(elem_matrix[i].mem == NULL || allocated_count == MAX_ELEM_SIZE) break;
        // check if available size if equal or greater than requested size
        if(elem_matrix[i].state == FREE && elem_matrix[i].size >= size) {
            if(alloc_method == QUICKEST) {
                bestmatchidx = i;
                break;
            } else {
                if(elem_matrix[i].size == size) {
                    bestmatchsize = elem_matrix[i].size;
                    bestmatchidx = i;
                    break;
                }
                else {
                    if(bestmatchsize >= elem_matrix[i].size) {
                        bestmatchsize = elem_matrix[i].size;
                        bestmatchidx = i;
                    }
                }
            }
        }
    }

    // if no matching size found return NULL
    if(bestmatchidx == -1) return NULL;

    // match found
    result = elem_matrix[bestmatchidx].mem;
    allocated_count ++;
    elem_matrix[bestmatchidx].state = OCCUPIED;
    balance_size = elem_matrix[bestmatchidx].size - size;
    elem_matrix[bestmatchidx].size = size;

    // if balance size then new free block is assigned
    if(balance_size) {
        last_entry ++;
        temp = last_entry-bestmatchidx;
        while(temp) {
            elem_matrix[temp+bestmatchidx] = elem_matrix[temp - 1 + bestmatchidx];
            temp --;
        }
        // allocate the new free block
        elem_matrix[bestmatchidx+1].mem = elem_matrix[bestmatchidx].mem + elem_matrix[bestmatchidx].size;
        elem_matrix[bestmatchidx+1].size = balance_size;
        elem_matrix[bestmatchidx+1].state = FREE;
    }
    return result;
}