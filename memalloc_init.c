#include "memalloc.h"
#include "malloc.h"

void memalloc_init() {
    elem_matrix[0].mem = &heap[0];
    elem_matrix[0].state = FREE;
    elem_matrix[0].size = HEAPSIZE;
}