#ifndef _MEMALLOC_H_
#define _MEMALLOC_H_

    #include "malloc.h"

    void embedded_malloc_init();
    unsigned char *embedded_malloc(unsigned short size, unsigned char alloc_method);

    extern unsigned char heap[HEAPSIZE];
    extern node_t elem_matrix[2*MAX_ELEM_SIZE];
    extern unsigned char last_entry;
    extern unsigned char allocated_count;

#endif