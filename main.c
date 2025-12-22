#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algos/includes/adt.h"
#include "util/includes/lecture.h"
#include "util/includes/gererMem.h"



// ----------------------------------------------------------------------------------------------------
// 2. Générateur de stats



// ----------------------------------------------------------------------------------------------------
// 3. Boucle d'exécution


void test(char * mot) {
    printf("Mot %s,", mot);
}

int main(int argc, char * argv[]) {
    FILE *fichier = lecture_fichier("data/ubu-roi.txt");

    
    fclose(fichier);


    lecture(fichier, &test);
}