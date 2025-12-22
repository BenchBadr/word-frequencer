#ifndef ADT_ALGO
#define ADT_ALGO

#include <stdio.h>
#include "../../util/includes/gererMem.h"

typedef struct _node {
    struct _node *droite;
    struct _node *gauche;
    struct _node *parent;
    int occ;
    char * mot;
} Node, * Tree;

void addToTree(InfoMem * infoMem, Tree * arbre, char * mot);
Node * initNode(InfoMem * infoMem);
void dispTree(Tree arbre, int maxDepth);

#endif