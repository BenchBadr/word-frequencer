#ifndef HASH_ALGO
#define HASH_ALGO

#include <stdint.h>
#include <string.h>
#include "../util/includes/gererMem.h"

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

// Return 64-bit FNV-1a hash for key (NUL-terminated).
static uint64_t hash_key(char *key);


typedef struct cell {
    struct cell *suiv;
    char * mot;
    int occ;
} Cell, *Liste;

typedef struct {
    Cell ** adrs;
    int capacity;
} Tableau;

Tableau initTableau(InfoMem * infoMem);

#endif