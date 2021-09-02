//
// Created by george on 9/2/21.
//

#include <string.h>
#include "functions.h"
#include "codes.h"

int endsWith(const char *str, const char *suffix){
    if (!str || !suffix)
        return FALSE;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return FALSE;

    if(strncmp(str + lenstr - lensuffix, suffix, lensuffix) != 0)
        return FALSE;

    return TRUE;
}