#include <time.h>
#include <stdlib.h>


int *ptr[5];

for(int i=0; i<5; i++) {
    ptr[i] = (int *)malloc(100);
}


clock_t begin = clock();

/* here, do your time-consuming job */

clock_t end = clock();
double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;