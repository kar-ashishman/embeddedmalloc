#include "malloc.h"
#include "memalloc.h"
#include <stdio.h>
#include <stdlib.h>


unsigned char *embedded_malloc(unsigned short size, unsigned char alloc_method);
void block_printer();


int main() {
    embedded_malloc_init();
    
    int temp;
    unsigned char* ptr[MAX_ELEM_SIZE], *tempptr;
    int ctr = 0;

    for(int i=0; i<52; i++) {
        temp = rand() % (50+1);
        temp = temp == 0 ? 13 : temp;
        tempptr = embedded_malloc(2, EFFICIENT);
        if(tempptr == (unsigned char*) NULL) {
            printf("%d - Cannot assign %d bytes\n", i, temp*2);
            continue;
        }
        *tempptr = temp*2;
        ptr[ctr] = tempptr;
        ctr++;
    }
}

void block_printer() {
    for(int i=0;i<=last_entry;i++)
        printf("(%d - %d - %d) ",i, elem_matrix[i].state, elem_matrix[i].size);
    printf("\n");
}