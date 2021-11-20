/* mem.h */
#ifndef SKETCH_CORE_MEM_H
#define SKETCH_CORE_MEM_H

#include <malloc.h>
#include <stdlib.h>

#define FREE_MEM(VAR) {                                         \
        free(VAR);                                              \
        VAR = NULL;                                             \
    }

#define FREE_ARRAY(ARRAY, SIZE) {                               \
        for (int i = 0; i < (SIZE); i++) free(ARRAY[i]);        \
        FREE_MEM(ARRAY)                                         \
    }


#define ALLOCATE_MEM(VAR, SIZE, SIZE_OF_TYPE) {                 \
            VAR = calloc(SIZE, SIZE_OF_TYPE);                   \
            if (VAR == NULL) {                                  \
                exit(-1);                                       \
            }                                                   \
    }

#define REALLOCATE_MEM(VAR, NEW_SIZE) {                         \
            VAR = realloc(VAR, NEW_SIZE);                       \
            if (VAR == NULL) {                                  \
                exit(-1);                                       \
            }                                                   \
    }



#endif
