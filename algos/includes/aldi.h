#ifndef ALDI_ALGO
#define ALDI_ALGO

#include <stdio.h>
#include "../../util/includes/gererMem.h"

typedef struct lc {
    struct lc *preced;
    struct lc *suiv;
    char * mot;
    int occ;
} Cellule, * Liste;

void addToListe(InfoMem * infoMem, Liste * liste, char * mot);
void fusionAldi(InfoMem * infoMem, Liste liste);
Cellule * initCellule(InfoMem * infoMem);

#endif