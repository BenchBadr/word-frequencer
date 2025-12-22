#ifndef ADT_ALGO
#define ADT_ALGO

#include <stdio.h>

typedef struct _node {
    struct _node *droite;
    struct _node *gauche;
} Tree;

void constructTree(Tree arbre, FILE * fichier);

#endif