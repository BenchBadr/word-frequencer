#include <stdint.h>
#include <stdlib.h>
#include "includes/hash.h"
#include <stdio.h>
#include <string.h>
#include "../util/includes/gererMem.h"

#define BUCKET_SIZE 10000


// Return 64-bit FNV-1a hash for key (NUL-terminated).
static uint64_t hash_key(char *key)
{
    uint64_t hash = FNV_OFFSET;
    for (char *p = key; *p != 0; p++) {
    hash ^= (uint64_t)(unsigned char)(*p);
    hash *= FNV_PRIME;
    }
    return hash;
}


static Cell * initCellule(InfoMem * infoMem, char * mot) {
    Cell * cellule = (Cell *) myMalloc(sizeof(Cell), infoMem);
    cellule->mot = (char *) myMalloc(strlen(mot) + 1, infoMem); 
    strcpy(cellule->mot, mot);
    cellule->occ = 1;
    cellule->suiv = NULL;
    return cellule;
}

void pushTableau(InfoMem * infoMem, Tableau * tableau, char * mot, int latex) {
    if (!tableau) return; // erreur malloc

    size_t index = hash_key(mot) % tableau->capacity;
    Cell * c = tableau->adrs[index];

    if (!c) {
        tableau->adrs[index] = initCellule(infoMem, mot);
        return;
    }

    while (c) {
        if (strcmp(c->mot, mot) == 0) {
            c->occ++;
            return;
        }
        if (c->suiv) {
            c = c->suiv;
        } else {
            break;
        }
    }

    c->suiv = initCellule(infoMem, mot);
}


Tableau initTableau(InfoMem * infoMem) {

    // cree les colonnes (pour collisions)
    ColliGroupe *liste = (Cell **)myMalloc(BUCKET_SIZE * sizeof(Cell *), infoMem);

    // a la maniere d'un calloc on met tout a NULL
    for (int i = 0; i < BUCKET_SIZE; i++) {
        liste[i] = NULL;
    }

    // cree le tableau
    Tableau tableau;
    tableau.adrs = liste;
    tableau.capacity = BUCKET_SIZE;
    return tableau;
}

static Cell * fusionne(Cell * a, Cell * b) {
    // cas triviaux
    if (!a) return b;
    if (!b) return a;


    Cell * resultat = NULL;
    Cell * curseur = NULL;

    while (a && b) {
        Cell * choix = NULL;

        if (a->occ >= b->occ) {
            choix = a;
            a = a->suiv;
        } else {
            choix = b;
            b = b->suiv;
        }

        if (!curseur) {
            resultat = choix;
            curseur = choix;
        } else {
            curseur->suiv = choix;
            curseur = choix;
        }
    }

    if (a) {
        curseur->suiv = a;
    } else if (b) {
        curseur->suiv = b;
    }


    return resultat;
}


static void split(Cell * source, Cell **part1, Cell **part2) {
    Cell *tortue = source;
    Cell *lievre = source;

    // le lièvre avance deux fois plus vite que la tortue
    while (lievre->suiv && lievre->suiv->suiv) {
        lievre = lievre->suiv->suiv;
        tortue = tortue->suiv;
    }

    *part1 = source;
    *part2 = tortue->suiv;

    // on coupe en 2 les deux parties
    tortue->suiv = NULL;
}

Cell * executerTriFusion(Cell * liste) {
    // Cas trivial
    if (!liste || !liste->suiv) {
        return liste;
    }

    Cell * a = NULL;
    Cell * b = NULL;

    // on casse la liste
    split(liste, &a, &b);


    // on trie recursivement chaque moitié
    a = executerTriFusion(a);
    b = executerTriFusion(b);

    // on fusionne
    return fusionne(a, b);
}




void fusionHash(Tableau *tableau, FILE *fichier) {
    if (!tableau) return;

    // On fusionne toutes les collisions en une liste chainee
    Cell * listeComplet = NULL;
    Cell * finPrec = NULL; // fin de la collision precedente pour accorder
    for (int i = 0; i < tableau->capacity; i++) {
        if (!tableau->adrs[i]) continue;

        Cell * c = tableau->adrs[i];
        if (!listeComplet) {
            listeComplet = c;
        } else {
            finPrec->suiv = c;
        }

        while (c->suiv) {
            c = c->suiv;
        }
        finPrec = c;
    }

    // on trie cette liste
    listeComplet = executerTriFusion(listeComplet);



    // on ecrit
    Cell * c = listeComplet;
    while (c) {
        fprintf(fichier, "%s: %d\n", c->mot, c->occ);
        c = c->suiv;
    }

}