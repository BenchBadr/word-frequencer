#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "includes/gererMem.h"



// ----------------------------------------------------------------------------------------------------
// Prérequis

void* myMalloc(size_t size, InfoMem* infoMem) {
    void* ptr = malloc(size);

    if (!ptr) {
        return NULL;
    }

    infoMem->cumul_alloc += size;
    return ptr;
}

void* myRealloc(void* ptr, size_t new_size, InfoMem* infoMem, size_t old_size) {
    void* newPtr = realloc(ptr, new_size);
    if (!newPtr) {
        return NULL;
    }
    infoMem->cumul_alloc += new_size - old_size;
    return newPtr;
}


void myFree(void* ptr, InfoMem* infoMem, size_t old_size) {
    infoMem->cumul_desalloc += old_size;
    free(ptr);

}
