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
    printf("%s\n", mot);
}

int main(int argc, char * argv[]) {

    InfoMem infoMem = {0,0};

    FILE *fichier = lecture_fichier("data/ubu-roi.txt");

    lecture(&infoMem, fichier, test);

}