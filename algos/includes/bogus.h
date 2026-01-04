#ifndef BOGUS_ALGO
#define BOGUS_ALGO

#include <stdio.h>
#include "../../util/includes/gererMem.h"

typedef struct Mot {
    char *mot;
    int occ;
} Mot;

typedef struct bogus {
    Mot ** elements;
    int nb_mots;
    int capacity;
} Bogus;

Bogus * initBogus(InfoMem * infoMem, FILE *fichier);
void addToBogus(InfoMem *infoMem, Bogus *bogus, char *mot, int latex);
void bulles_and_write(Bogus *bogus, FILE *fichier);

#endif