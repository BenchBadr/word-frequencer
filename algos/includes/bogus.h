#ifndef BOGUS_ALGO
#define BOGUS_ALGO

#include <stdio.h>
#include "../../util/includes/gererMem.h"

typedef struct {
    char * mot;
    int occ;
} BogusCell;

typedef struct {
    BogusCell *entries;
    int count;
    int capacity;
} BogusArray;

BogusArray* initBogusArray(InfoMem *infoMem, FILE fichier);
void addToBogus(BogusArray *arr, char *mot, InfoMem *infoMem);
void bubbleSort(BogusArray *arr);
void writeBogus(BogusArray *arr);
void freeBogus(BogusArray *arr, InfoMem *infoMem);

#endif