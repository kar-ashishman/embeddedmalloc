#ifndef _MALLOC_H_
#define _MALLOC_H_

    #define EFFICIENT     0
    #define QUICKEST      1

    #define HEAPSIZE 2000

    #define MAX_ELEM_SIZE   50
    #define OCCUPIED        1
    #define FREE            0

    struct Node {
        unsigned short size;
        unsigned char *mem;
        unsigned char state;
    };

    typedef struct Node node_t;

#endif