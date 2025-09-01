#include <stdio.h>
#include <stdlib.h>
#include "malloc.h"


unsigned char heap[HEAPSIZE] = {5};


//#define NULL            (void *)0x0

node_t elem_matrix[2*MAX_ELEM_SIZE];
unsigned char last_entry = 0;
unsigned char allocated_count = 0;

void init_elem_matrix() {
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


void embedded_free(unsigned char* mem) {
    int leftfree = 0, rightfree = 0, i,j,n;
    for(i=0; i<last_entry; i++) {
        // check if block memory is same as requested memory region to be freed
        if(elem_matrix[i].mem == mem && elem_matrix[i].state == OCCUPIED) {
            elem_matrix[i].state = FREE;
            allocated_count --;
            // check if the right block is free
            if(i+1 <= last_entry && elem_matrix[i+1].state == FREE) {
                elem_matrix[i].size += elem_matrix[i+1].size;
                rightfree = 1;
            }
            // check if the left block is free
            if(i>=1 && elem_matrix[i-1].state == FREE) {
                // increase the left free blocks size
                elem_matrix[i-1].size += elem_matrix[i].size;
                leftfree = 1;
            }
            break;
        }
    }
    // if left or right of current free block is found to be free, move the element matrix left
    if (leftfree || rightfree) {
        if(leftfree && !rightfree) {j = i+1; n = 1;}
        else if(rightfree && !leftfree) {j = i+2; n = 1;}
        else {j = i+2; n=2;}
        for(;j<=last_entry;j++) {
            elem_matrix[j-n] = elem_matrix[j];
        }
        elem_matrix[last_entry].mem = 0;
        elem_matrix[last_entry].state = 0;
        elem_matrix[last_entry].size = 0;
        if(n > 1) {
            elem_matrix[last_entry-1].mem = 0;
            elem_matrix[last_entry-1].state = 0;
            elem_matrix[last_entry-1].size = 0;
        }
        last_entry -= n;
    }

}

unsigned char* embedded_calloc(int n, int size, int method) {
    int total_size = n*size;
    unsigned char* ptr = embedded_malloc(total_size, method);
    if(ptr == NULL) return NULL;
    for(int i=0; i<total_size; i++) {
        *(ptr+i) = 0;
    }
    return ptr;
}

unsigned char* embedded_realloc(unsigned char* ptr, int size, int method) {
    // If possible, realloc() attempts to resize the existing memory block at the same address.
    // If the current location does not have enough contiguous space for the new size,
    // realloc() allocates a new, larger block of memory, copies the contents of the old block to the new block
    // (up to the minimum of the old and new sizes), and then frees the old block.

    return NULL;
}


void block_printer() {
    for(int i=0;i<=last_entry;i++)
        printf("(%d - %d - %d) ",i, elem_matrix[i].state, elem_matrix[i].size);
    printf("\n");
}


int main() {
    init_elem_matrix();
    unsigned char* ptr[50], *tempptr;
    int temp;
    int ctr = 0;
    for(int i=0; i<52; i++) {
        temp = rand() % (50+1);
        temp = temp == 0 ? 13 : temp;
        tempptr = embedded_calloc(2, temp, EFFICIENT);
        if(tempptr == (unsigned char*) NULL) {
            printf("%d - Cannot assign %d bytes\n", i, temp*2);
            continue;
        }
        *tempptr = temp*2;
        ptr[ctr] = tempptr;
        ctr++;
    }
    printf("%d \n", last_entry);
    block_printer();
    printf("\nRemaining memory - %d\n",elem_matrix[last_entry].size);

    for(int i=0; i<last_entry/2; i++) {
        temp = rand() % last_entry;
        printf("%d (%d)--> ", temp, *(ptr[temp]));
        if(temp == 40)
            temp --;
        if(temp == 40)
            temp ++;
        embedded_free(ptr[temp]);
        block_printer();
        printf("\nRemaining memory - %d\n",elem_matrix[last_entry].size);
    }

    embedded_malloc(5, EFFICIENT);

    printf("test\n");
    block_printer();
    return 0;
}