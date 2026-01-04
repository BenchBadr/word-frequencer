#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algos/includes/adt.h" 
#include "algos/includes/aldi.h" 
#include "algos/includes/bogus.h" 
#include "algos/includes/hash.h" 

#include "util/includes/lecture.h" 
#include "util/includes/gererMem.h" 
#include <string.h>

/*
1 : BOGUS
2 : ALDI
3 : ADT
4 : HASH
*/

#define MAX_FILES 5

#define BASE_CAPACITY 20000




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

void stats(InfoMem infoMem, int latex) {

    char * pref = latex ? "\\item" : "- ";

    if (latex) {
        printf("\\newpage \\ \\section{Statistiques d'exécution}\n\n\\begin{itemize}\n");
    } else {
        repeat('-', 50);
        printf("# Statistiques d'exécution\n\n");
    }
    printf("%s Mémoire totale allouée : %zu octets\n", pref, infoMem.cumul_alloc);
    printf("%s Mémoire totale désallouée : %zu octets\n", pref, infoMem.cumul_desalloc);
    printf("%s Pic d'allocations : %zu octets\n", pref, infoMem.max_alloc);
    printf("%s Nombre de mots distincts : %d mots\n", pref, infoMem.nbMotsDist);
    printf("%s Nombre de mots traités : %d mots\n", pref, infoMem.nbMots);

    printf("%s Temps d'exécution : %f secondes\n", pref, getElapsed(infoMem.start));
    if (latex) {
        printf("\\begin{itemize}\\item \\textit{NB : Performances dégradée par la génération du visuel et les affichages l'accompagnant.}\\end{itemize}");
        printf("\\end{itemize}\n");
    } else {
        printf("\t NB : Performances dégradée par la génération du visuel et les affichages l'accompagnant\n");
    }
}

void grapher(int dim, int option, int approche, int min_lettre) {
    /*
    Dim : grille
    Option :
        0 : Temps / mots
        1 : Memoire / mots
    */
    FILE *fichier = lecture_fichier("data/ubu-roi.txt");
    FILE *out = fopen("data/.tests.txt", "w");
    int latex = 0;

    printf("\\vspace{3cm}\n\\ \n");
    printf("\\chart{%d}{(0,0) ",dim);

    int ratioer;
    int nbMots;

    for (int i = 0; i < dim + 1; i++) {

        InfoMem infoMem = {0, 0, 0, clock(), 0}; 
        void *adr;

        if (approche == 3 || approche == 2) {
            adr = NULL;
        }
        if (approche == 1) {
            adr = initBogus(&infoMem, fichier);
        }
        for (int j = 0; j <= i; j++) { 

            if (approche == 3) {
                lecture(&infoMem, fichier, (void (*)(InfoMem *, void *, char *, int))addToTree, &adr, latex, min_lettre);
            }
            if (approche == 2) {
                lecture(&infoMem, fichier, (void (*)(InfoMem *, void *, char *, int))addToListe, &adr, latex, min_lettre);
            }
            if (approche == 1) {
                lecture(&infoMem, fichier, (void (*)(InfoMem *, void *, char *, int))addToBogus, adr, latex, min_lettre);
            }
            rewind(fichier);
        }

        if (approche == 3) {
            writeTree((Tree)adr, &infoMem, out); 
        }
        if (approche == 2) {
            fusionAldi(&infoMem, (Liste)adr, out, latex);
        }
        if (approche == 1) {
            bulles_and_write((Bogus *)adr, out);
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

    /*
    1. Lectures des arguments
    */

    int approche = 3;
    int latex = 0;
    char * sortie_path = "sorties/out.txt";
    char * entrees[MAX_FILES];
    int curs_ent = 0;

    int graph = 0;

    int min_lettre = 1;


    // TODO : Remplacer par des switch cases
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            if (strcmp("bogus",argv[i+1]) == 0) {
                approche = 1;
            } else if (strcmp("aldi",argv[i+1]) == 0) {
                approche = 2;
            } else if (strcmp("adt",argv[i+1]) == 0) {
                approche = 3;
            } else if (strcmp("hash",argv[i+1]) == 0) {
                approche = 4;
            } else {
                printf("Error: chosen algorithm unrecognized. \nType `help` to learn more.\n");
                return 1;
            }

            i++;
            continue;
        }

        if (strcmp(argv[i], "-help") == 0) {
            repeat('=', 100);

            printf("Options (and corresponding environment variables):\n");

            // option -a / Algos
            printf("-a\t : Algorithm choice. Please refer to `rapport.pdf` to learn more about each. \n \t   Valid options : `bogus`, `aldi`, `adt`, `hash`\n\t   Default : `adt`\n");

            
            // option -out
            printf("-out\t : Specify produced text output file. Default : `sorties/out.txt`\n\t.\n");
            
            // option -l 
            printf("-l\t : Latex mode. Produces debugging utility written in `examples/tests/out.tex`.\n\t   Default : unset\n");

            // option -g
            printf("-g\t : Create a graph of the execution (such as shown in `examples/chart.tex`). Will return both curve for time evolution and memory usage evolution.\n");

            // option -ml
            printf("-ml\t : Specify the minimum of letters per word to be considered. (default : 1)\n");

            // option -help
            printf("-help\t : Learn more about the options.\n");

            // entree
            printf("Files\t : Specify path of the input files. Max : %d\n\t   Default : `data/hittites.txt`\n", MAX_FILES);

            repeat('=', 100);
            
            continue;
        }

        if (strcmp(argv[i], "-l") == 0) {
            latex = 1;
            continue;
        }

        if (strcmp(argv[i], "-g") == 0) {
            graph = 1;
            continue;
        }
        
        if (strcmp(argv[i], "-out") == 0 && i + 1 < argc) {
            sortie_path = argv[i+1];

            i++;
            continue;
        }

        if (strcmp(argv[i], "-ml") == 0 && i + 1 < argc) {
            min_lettre = atoi(argv[i+1]);

            i++;
            continue;
        }

        // lecture fichier
        if (i) {
            entrees[curs_ent] = argv[i];
            curs_ent++;
        }
    }


    if (!curs_ent) {
        entrees[0] = "data/hittites.txt";
        curs_ent++;
    }

    if (graph) {
        printf("\\section{Rendement}\n");
        printf("\\textbf{Courbe d'évolution du temps d'exécution}\n");
        grapher(50, 0, approche, min_lettre);

        printf("\\textbf{Courbe d'évolution de l'utilisation de la mémoire}\n");
        grapher(10, 1, approche, min_lettre);
        return 0;
    }

    char * pref = !latex ? "#" : "\\section{";
    char * suff = !latex ? "" : "}";

    for (int i = 0; i < curs_ent; i++) {
        if (!entrees[i]) return 1;
        InfoMem infoMem = {0,0, 0, clock(), 0};

        if (!latex) {
            repeat('-', 50);
            printf("Fichier : %s \n\n", entrees[i]);
        }
        FILE *fichier = lecture_fichier(entrees[i]);
        FILE *sortie = fopen(sortie_path, "w");

        if (approche == 1) {
            printf("%s Approche : Bogus%s\n\n\n", pref, suff);
            Bogus *bogus = initBogus(&infoMem, fichier);

            lecture(&infoMem, fichier, (void (*)(InfoMem *, void *, char *, int))addToBogus, bogus, latex, min_lettre);

            bulles_and_write(bogus, sortie);
        }

        if (approche == 2) {
            printf("%s Approche : ALDI%s\n\n\n", pref, suff);
            Liste liste = NULL;

            lecture(&infoMem, fichier, (void (*)(InfoMem *, void *, char *, int))addToListe, &liste, latex, min_lettre);

            fusionAldi(&infoMem, liste, sortie, latex);
        } 

        if (approche == 3) {
            printf("%s Approche : ADT%s\n\n\n", pref, suff);
            Tree arbre = NULL;
            lecture(&infoMem, fichier, (void (*)(InfoMem *, void *, char *, int))addToTree, &arbre, latex, min_lettre);

            writeTree(arbre, &infoMem, sortie);

        }
        if (approche == 4) {
            printf("%s Approche : HASH%s\n\n\n", pref, suff);
            Tableau bucket = initTableau(&infoMem);
            lecture(&infoMem, fichier, (void (*)(InfoMem *, void *, char *, int))pushTableau, &bucket, latex, min_lettre);
            fusionHash(&bucket, sortie);
        }

        stats(infoMem, latex);
    }   
}