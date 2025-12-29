
#include <stdio.h>
#include <string.h>
#include "../util/includes/gererMem.h"
#include "includes/aldi.h"


// Distance simple - À remplacer par Levenstein ultérieurement
int distance(const char *s1, const char *s2){
    int i = 0, count = 0;
    while(s1[i] && s2[i]){
        if(s1[i] != s2[i]) count++;
        i++;
    }

    while(s1[i]){ count++; i++; }
    i = 0;
    while(s2[i]){ count++; i++; }
    return count;
}


Cellule * initCellule(InfoMem * infoMem) {
    Liste ptr = (Cellule *) myMalloc(sizeof(Cellule), infoMem);
    ptr->occ = 1;
    return ptr;
}

void addToListe(InfoMem * infoMem, Liste * liste, char * mot) {
    if (!*liste) {
        *liste = initCellule(infoMem);
        (*liste)->mot = mot;
        (*liste)->suiv = (*liste);
        (*liste)->preced = (*liste);
    }

    // on choisit la direction
    int d1 =((*liste)->preced && (*liste)->preced != *liste) ? distance(mot, (*liste)->preced->mot) : -1;
    int d2 = distance(mot, (*liste)->mot);

    Cellule * c = (*liste)->suiv;

    // direction usuelle - gauche à droite
    int cmp = d1 < 0 ? 1 :  d1 > d2 ? -1 : 1;



    while (cmp * strcmp(mot, c) < 0 && c != *liste)  {
        if (cmp > 0) {
            c = c->suiv;
        } else {
            c = c->preced;
        }
    }

    if (!*liste) {
    fprintf(stderr, "Error: liste is NULL\n");
    return;
}
    return;

    // noeud a ajouter
    if (c == *liste) {
        Cellule * nptr = initCellule(infoMem);


        if (cmp > 0) {
            nptr->preced = c->preced;
            nptr->suiv = (*liste);
            (*liste)->preced = nptr;
        } else {
            nptr->suiv = c->preced;
            nptr->preced = (*liste);
            (*liste)->suiv = nptr;
        }
    }

    // meme mot
    if (c->mot == mot) {
        c->occ++;
    }


}

