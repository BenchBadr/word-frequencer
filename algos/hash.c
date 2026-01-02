#include <stdint.h>
#include "includes/hash.h"
#include <string.h>
#include "../util/includes/gererMem.h"

#define BUCKET_SIZE 32768 // 2^15

// Return 64-bit FNV-1a hash for key (NUL-terminated).
static uint64_t hash_key(char *key)
{
    uint64_t hash = FNV_OFFSET;
    for (char *p = key; *p != 0; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
    }
    return hash;
}


static Cell * initCellule(InfoMem * infoMem, char * mot) {
    Cell * cellule = (Cell *) myMalloc(sizeof(Cell), infoMem);
    cellule->mot = (char *) myMalloc(strlen(mot) + 1, infoMem); 
    strcpy(cellule->mot, mot);
    return cellule;
}

static void pushTableau(InfoMem * infoMem, Tableau * tableau, char * mot) {
    size_t index = hash_key(mot) % tableau->capacity;
    Cell * c = tableau->adrs[index];
    while (c && c->suiv) {
        if (strcmp(c->mot, mot) == 0) {
            c->occ++;
            return;
        }
        c = c->suiv;
    }

    if (strcmp(c->mot, mot) == 0) {
        c->occ++;
    } else {
        c->suiv = initCellule(infoMem, mot);
    }

}



Tableau initTableau(InfoMem * infoMem) {

    // cree les colonnes (pour collisions)
    Liste *liste = (Cell **)myMalloc(BUCKET_SIZE * sizeof(int), infoMem);

    // cree le tableau
    Tableau tableau;
    tableau.adrs = liste;
    tableau.capacity = BUCKET_SIZE;
    return tableau;
}