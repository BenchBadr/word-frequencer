#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    size_t cumul_alloc;
    size_t cumul_desalloc; 
} InfoMem;


// ----------------------------------------------------------------------------------------------------
// Prérequis

void* myMalloc(size_t size, InfoMem* infoMem) {
    void* ptr = malloc(size);

    if (!ptr) {
        return;
    }

    infoMem->cumul_alloc += size;
    return ptr;
}

void* myRealloc(void* ptr, size_t new_size, InfoMem* infoMem, size_t old_size) {
    void* ptr = realloc(ptr, new_size);
    if (!ptr) {
        return;
    }
    infoMem->cumul_alloc += new_size - old_size;
    return ptr;
}


void myFree(void* ptr, InfoMem* infoMem, size_t old_size) {
    infoMem->cumul_desalloc += old_size;
    free(ptr);

}