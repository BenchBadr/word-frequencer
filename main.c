#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algos/includes/adt.h" 
#include "algos/includes/aldi.h" 

#include "util/includes/lecture.h" 
#include "util/includes/gererMem.h" 

/*
3 : ADT
*/

#ifndef APPROCHE
    #define APPROCHE 3
#endif





// ----------------------------------------------------------------------------------------------------
// 2. Générateur de stats

void repeat(char c, int n) {
    for (int i = 0; i < n;i++) printf("%c",c);
    printf("\n");
}

double getElapsed(clock_t start) {
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    return elapsed;
}

void stats(InfoMem infoMem) {
    printf("\\newpage \\ \\section{Statistiques d'exécution}\n\n\\begin{itemize}\n");
    printf("\\item Mémoire totale allouée : %zu octets\n", infoMem.cumul_alloc);
    printf("\\item Mémoire totale désallouée : %zu octets\n", infoMem.cumul_desalloc);
    printf("\\item Pic d'allocations : %zu octets\n", infoMem.max_alloc);
    printf("\\item Nombre de mots distincts : %d mots\n", infoMem.nbMotsDist);
    printf("\\item Nombre de mots traités : %d mots\n", infoMem.nbMots);


    printf("\\item Temps d'exécution : %f secondes\n", getElapsed(infoMem.start));
    printf("\\begin{itemize}\\item \\textit{NB : Performances dégradée par la génération du visuel et les affichages l'accompagnant.}\\end{itemize}");
    printf("\\end{itemize}\n");
}

void grapher(int dim, int option) {
    /*
    Dim : grille
    Option :
        0 : Temps / mots
        1 : Memoire / mots
    */
    FILE *fichier = lecture_fichier("data/albatros.txt");


    printf("\\newpage\n\\ \n");
    printf("\\chart{%d}{",dim);

    int ratioer;
    int nbMots;

    for (int i = 0; i < dim + 1; i++) {

        // pour l'adt
        Tree adr = NULL;
        InfoMem infoMem = {0, 0, clock(), 0, 0}; 
        for (int j = 0; j <= i; j++) { 

            if (APPROCHE == 3) {
                lecture(&infoMem, fichier, (void (*)(InfoMem *, void *, char *))addToTree, &adr);
            }
            rewind(fichier);
        }

        if (APPROCHE == 3) {
            writeTree(adr, &infoMem); 
        }
        if (i == 0) {
            ratioer = infoMem.max_alloc;
            nbMots = infoMem.nbMots;
        }
        printf("(%d,%f)", i, 
            option == 0 ? getElapsed(infoMem.start)*100 : (
                (double)infoMem.max_alloc / ratioer
            ));
    }
    printf("}");
    option == 0 ? printf("{Nb. mots (1/%d)}{Temps d'ex. (s)}", nbMots) : (
        printf("{Nb mots (1/%d)}{Max alloc (1/%d)}", nbMots, ratioer)
    );
    printf("\n");

    fclose(fichier);
}

// ----------------------------------------------------------------------------------------------------
// 3. Boucle d'exécution


void afficher(InfoMem * infoMem, void * adr, char * mot) {
    printf("%s", mot);
}


int main(int argc, char * argv[]) {

    InfoMem infoMem = {0,0, clock(), 0, 0};
    Tree liste = NULL;

    FILE *fichier = lecture_fichier("data/hittites.txt");

    lecture(&infoMem, fichier, (void (*)(InfoMem *, void *, char *))addToTree, &liste);

    stats(infoMem);
    // grapher(5, 0);
    // grapher(5, 1);
}