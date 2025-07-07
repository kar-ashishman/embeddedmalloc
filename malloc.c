#include <stdio.h>

unsigned char heap[2000] = {5};

struct Node {
    unsigned short size;
    unsigned char state;
    unsigned char *mem;
    struct Node *next;
    struct Node *prev;
};

typedef struct Node node_t;


#define MAX_ELEM_SIZE   50
#define OCCUPIED        1
#define FREE            0   
//#define NULL            (void *)0x0   

node_t elem_matrix[2*MAX_ELEM_SIZE];
unsigned char last_entry = 0;
unsigned char allocated_count = 0;

void init_elem_matrix() {
    elem_matrix[0].size = 2000;
    elem_matrix[0].state = FREE;
    elem_matrix[0].mem = &heap[0];
    elem_matrix[0].next = NULL;
    elem_matrix[0].prev = NULL;
}

unsigned char *embedded_malloc(unsigned short size) {

    // Can not allocate more than MAX_ELEM_SIZE blocks
    if (allocated_count > MAX_ELEM_SIZE) return NULL;

    unsigned char free = 0, i = 0;

    // Check if any block with adequate size is available
    for(; i < 2*MAX_ELEM_SIZE; i++) {
        if(elem_matrix[i].state == FREE && elem_matrix[i].size >= size) {
            free = 1;
            break;
        }
    }

    // Return NULL if no free block is found
    if (!free) {
        return NULL;
    }


    unsigned short available_size = elem_matrix[i].size;
    last_entry ++;
    allocated_count ++;

    // Allocate block
    elem_matrix[i].size = size;
    elem_matrix[i].state = OCCUPIED;
    elem_matrix[i].next = NULL;

    if (available_size > size) {
        elem_matrix[last_entry].size = available_size - size;
        elem_matrix[last_entry].state = FREE;
        elem_matrix[last_entry].mem = elem_matrix[i].mem + size;
        elem_matrix[last_entry].next = NULL;
        elem_matrix[last_entry].prev = &elem_matrix[i];
        elem_matrix[i].next = &elem_matrix[last_entry];
    }
    
    return elem_matrix[i].mem;
}

char embedded_free(unsigned char *ptr) {
    node_t *node = &elem_matrix[0];
    node_t **temp;
    node_t **temp1;
    node_t **temp2;
    char found = 0;
    while(node != NULL) {
        if (node->next == NULL) break;
        if (node->mem == ptr) {
            found = 1;
            break;
        }
        node = node->next;
    }

    if(!found) {
        return -1; // Pointer not found
    }

    node->state = FREE;
    allocated_count --;

    // Merge with left neighbor if it is free
    if (node->prev->state == FREE) {
        node->prev->size += node->size;
        node->prev->next = node->next;

        // shift last occupied entry to node
        temp = &node;
        temp1 = &node->prev;
        **temp = elem_matrix[last_entry - 1];

        // shift last block free memory one entry back
        elem_matrix[last_entry - 1] = elem_matrix[last_entry];
        last_entry --;
    }

    // Merge with right neighbor if it is free
    if ((*temp1)->next->state == FREE) {
        temp2 = &(*temp1)->next;

        (*temp1)->size += (*temp1)->next->size;
        (*temp1)->next = (*temp1)->next->next;

        // shift last occupied entry to node
        **temp2 = elem_matrix[last_entry - 1];

        // shift last block free memory one entry back
        elem_matrix[last_entry - 1] = elem_matrix[last_entry];
        last_entry --;
    }

}

void block_printer() {
    printf("\n");
    node_t *node = &elem_matrix[0];
    while(node != NULL) {
        printf("\nState = %d, Size = %d, 1st Data = %c", node->state, node->size, *node->mem);
        node = node->next;
    }
    printf("Last Entry = %d, Allocated Count = %d", last_entry, allocated_count);
    printf("\n");
}


int main() {
    init_elem_matrix();

    // 2b 2b 10b 3b 5b 7b 10b
    unsigned char*    block1 = embedded_malloc(2);
    block1[0] = 'A';

    unsigned char*    block2 = embedded_malloc(2);
    block2[0] = 'B';

    unsigned char*    block3 = embedded_malloc(10);
    block3[0] = 'C';

    unsigned char*    block4 = embedded_malloc(3);
    block4[0] = 'D';

    unsigned char*    block5 = embedded_malloc(5);
    block5[0] = 'E';

    unsigned char*    block6 = embedded_malloc(7);
    block6[0] = 'F';

    unsigned char*    block7 = embedded_malloc(10);
    block7[0] = 'G';

    block_printer();

    //printf("\nFreeing blocks...\n");
    
    embedded_free(block2); // Free B
    embedded_free(block4); // Free D
    embedded_free(block3); // Free C

    block_printer();
}