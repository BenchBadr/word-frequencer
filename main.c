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



int main(int argc, char * argv[]) {

    InfoMem infoMem = {0,0};
    Tree arbre = NULL;

    FILE *fichier = lecture_fichier("data/albatros.txt");

    lecture(&infoMem, fichier, addToTree, &arbre);
    dispTree(arbre, 5);
}