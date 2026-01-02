// filepath: /Users/arl/prog/eiffel/TP Infos/word-frequencer/algos/bogus.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../util/includes/gererMem.h"
#include "includes/bogus.h"

#ifndef LATEX_MODE
    #define LATEX_MODE 0
#endif

typedef struct {
    char *mot;
    int occ;
} Entry;

typedef struct {
    Entry *entries;
    int count;
    int capacity;
} BogusArray;
