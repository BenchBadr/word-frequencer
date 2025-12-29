#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "includes/gererMem.h"



// ----------------------------------------------------------------------------------------------------
// Prérequis

static void updateMax(InfoMem* infoMem) {
    size_t current = infoMem->cumul_alloc - infoMem->cumul_desalloc;
    if (current > infoMem->max_alloc)
        infoMem->max_alloc = current;
}

void* myMalloc(size_t size, InfoMem* infoMem) {
    void* ptr = malloc(size);
    if (!ptr) return NULL;

    infoMem->cumul_alloc += size;
    updateMax(infoMem);
    return ptr;
}

void myFree(void* ptr, InfoMem* infoMem, size_t old_size) {
    if (!ptr) return;

    infoMem->cumul_desalloc += old_size;
    free(ptr);
}

void* myRealloc(void* ptr, size_t new_size, InfoMem* infoMem, size_t old_size) {
    void* newPtr = realloc(ptr, new_size);
    if (!newPtr) return NULL;

    // pas de deplacement
    // ajout de diff
    if (newPtr == ptr) {
        if (new_size > old_size)
            infoMem->cumul_alloc += new_size - old_size;
        else // deplacement - tout rajouter et free l'antecedant
            infoMem->cumul_desalloc += old_size - new_size;
    } else {
        infoMem->cumul_alloc += new_size;
        infoMem->cumul_desalloc += old_size;
    }

    updateMax(infoMem);
    return newPtr;
}
