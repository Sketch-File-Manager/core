#ifndef SKETCH_CORE_MEM_H
#define SKETCH_CORE_MEM_H

#include <malloc.h>

#define FREE_ARRAY(ARRAY, SIZE) do {                                \
        for (int i = 0; i < (SIZE); i++) free(ARRAY[i]);            \
        free(ARRAY);                                                \
                                                                    \
} while (0)

#define ALLOCATE_MEM(ITEM, SIZE, TYPE) do {                         \
        (ITEM) = calloc((SIZE), sizeof((TYPE));                     \
        if ((ITEM) == NULL)                                         \
                // TODO - Do something.                             \
                                                                    \
} while(0)

#define REALLOCATE_MEM(ITEM, NEW_SIZE, TYPE) do {                   \
        (ITEM) = realloc((ITEM), sizeof(TYPE) * NEW_SIZE)           \
        if ((ITEM) == NULL)                                         \
                // TODO - Do something.                             \
} while(0)


#endif
