#ifndef SKETCH_CORE_MEM_H
#define SKETCH_CORE_MEM_H

#include <malloc.h>
#include <stdlib.h>

#define FREE_ARRAY(ARRAY, SIZE) do {                                \
        for (int i = 0; i < (SIZE); i++) free(ARRAY[i]);            \
        free(ARRAY);                                                \
                                                                    \
} while (0)


#define ALLOCATE_MEM(VAR, SIZE, SIZE_OF_TYPE) do {                  \
            (VAR) = calloc(SIZE, SIZE_OF_TYPE);                     \
            if ( (VAR) == NULL ) {                                  \
                exit(1);                                            \
            }                                                       \
    } while(0)

#define REALLOCATE_MEM(VAR, NEW_SIZE) do {                          \
            (VAR) = realloc((VAR), NEW_SIZE);                       \
            if ( (VAR) == NULL ) {                                  \
                exit(1);                                            \
            }                                                       \
    } while(0)



#endif
