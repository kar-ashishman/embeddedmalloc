#include "malloc.h"

unsigned char heap[HEAPSIZE];
node_t elem_matrix[2*MAX_ELEM_SIZE];
unsigned char last_entry = 0;
unsigned char allocated_count = 0;