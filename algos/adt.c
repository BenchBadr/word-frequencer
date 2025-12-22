#include "includes/adt.h"
#include <string.h>
#include "../util/includes/gererMem.h"

Tree initNode(InfoMem * infoMem) {
    Tree ptr = (Tree) myMalloc(sizeof(Node), infoMem);
    ptr->droite = NULL;
    ptr->gauche = NULL;
    ptr->parent = NULL;
    ptr->occ = 1;
    return ptr;
}

void saison(Node *t) {
    // une saison casse l'ordre alphabetique
    if (!t || !t->parent) return;

    Node * p = t->parent;

    if (t->occ > 2 * p->occ) {
        t->parent = p->parent;

        if (p->parent) {
            if (p->parent->droite == p) {
                p->parent->droite = t;
            } else {
                p->parent->gauche = t;
            }
        }


        p->parent = t;

        if (p->droite == t) {
            p->droite = t->droite;
            t->droite = p;
        } else {
            p->gauche = t->gauche;
            t->gauche = p;
        }


        // il faut encore recurser (optionel)
    }
}




void addToTree(InfoMem * infoMem, Tree * arbre, char * mot) {
    printf("Mot : %s\n", mot);
    Tree * t = arbre;
    Node * par = NULL; // pour stocker le parent
    while (*t) {
        int res = strcmp((*t)->mot, mot);
        par = (Node *) *t; 
        if (res == 0) {
            (*t)->occ++;
            saison(*t);
            return;
        } else if (res > 0) {
            t = &((*t)->gauche);
        } else {
            t = &((*t)->droite);
        }
    }
    *t = initNode(infoMem);
    (*t)->parent = (Node *) par;

    char *copy = myMalloc((strlen(mot) + 1), infoMem);
    strcpy(copy, mot);
    (*t)->mot = copy;
}


void dispTreeRec(Tree arbre, int maxDepth, int currentDepth) {
    if (!arbre || currentDepth >= maxDepth) {
        return;
    }
    
    if (currentDepth == 0) {
        printf("\\node ");
    } else {
        printf("node");
    }

    printf(" {\\small (%s, %d)} \n", arbre->mot, arbre->occ);

    if (arbre->gauche && currentDepth+1 < maxDepth) {
        printf("    child {");
        dispTreeRec(arbre->gauche, maxDepth, currentDepth+1);
        printf("}");
    }

    if (arbre->droite && currentDepth+1 < maxDepth) {
        printf("    child {");
        dispTreeRec(arbre->droite, maxDepth, currentDepth+1);
        printf("}");
    }
}

void dispTree(Tree arbre, int maxDepth){
    printf("\\begin{document}\n");
    printf("\\begin{tikzpicture}\n");
    printf("[\n");
    printf("    level 1/.style={sibling distance=55mm},\n");
    printf("    level 2/.style={sibling distance=35mm},\n");
    printf("]\n");

    dispTreeRec(arbre, maxDepth, 0);

    printf(";\n\\end{tikzpicture}\n");
    printf("\\end{document}\n");
}