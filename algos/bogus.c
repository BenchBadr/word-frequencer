#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../util/includes/gererMem.h"
#include "includes/bogus.h"

int chercher_mot(Bogus *bogus, const char *mot) {
    for (int i = 0; i < bogus->nb_mots; i++) {
        if (strcmp(bogus->elements[i]->mot, mot) == 0) {
            return i;
        }
    }
    return -1;
}

Mot * initMot(InfoMem * infoMem) {
    Mot *ptr = (Mot *) myMalloc(sizeof(Mot), infoMem);
    ptr->mot = NULL;
    ptr->occ = 0;
    return ptr;
}

Bogus * initBogus(InfoMem * infoMem, FILE *fichier) {
    long file_size = ftell(fichier);
    fseek(fichier, 0, SEEK_END);
    file_size = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);
    int capacity = file_size / 2;

    Bogus *ptr = (Bogus *) myMalloc(sizeof(Bogus), infoMem);
    ptr->capacity = capacity;
    ptr->nb_mots = 0;
    ptr->elements = (Mot **) myMalloc(sizeof(Mot *) * ptr->capacity, infoMem);
    return ptr;
}

void update_capacity(InfoMem *infoMem, Bogus *bogus) {
    if (bogus->nb_mots + 1 == bogus->capacity) {
        bogus->capacity *= 2;
        bogus->elements = (Mot **) myRealloc(bogus->elements, 2 * sizeof(Mot *) * bogus->capacity, infoMem, sizeof(Mot *) * bogus->capacity);
    }
}

void addToBogus(InfoMem *infoMem, Bogus *bogus, char *mot, int latex) {
    int idx = chercher_mot(bogus, mot);
    if (idx != -1) {
        bogus->elements[idx]->occ++;
    } else {
        if (bogus->nb_mots >= bogus->capacity) {
            update_capacity(infoMem, bogus);
        }
        Mot *new_mot = initMot(infoMem);
        new_mot->mot = (char *)myMalloc(strlen(mot) + 1, infoMem);
        strcpy(new_mot->mot, mot);
        new_mot->occ = 1;
        bogus->elements[bogus->nb_mots] = new_mot;
        bogus->nb_mots++;
    }
}

void bulles_and_write(Bogus *bogus, FILE *fichier) {
    Mot **tab = bogus->elements;
    int nb_mots = bogus->nb_mots;
    
    for (int i = 0; i < nb_mots; i++) {
        for (int j = 0; j < nb_mots - 1; j++) {
            if (tab[j]->occ < tab[j + 1]->occ) {
                Mot *tmp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = tmp;
            }
        }
    }

    for (int i = 0; i < nb_mots; i++) {
        fprintf(fichier,"%s %d\n", tab[i]->mot, tab[i]->occ);
    }
}