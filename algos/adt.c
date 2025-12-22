#include "includes/adt.h"
#include <string.h>
#include "../util/includes/gererMem.h"

Tree initNode(InfoMem * infoMem) {
    Tree ptr = (Tree) myMalloc(sizeof(Node), infoMem);
    ptr->droite = NULL;
    ptr->gauche = NULL;
    ptr->occ = 0;
    return ptr;
}

void addToTree(InfoMem * infoMem, Tree * arbre, char * mot) {
    *arbre = initNode(infoMem);
    (*arbre)->mot = "root";
    (*arbre)->occ = 5;
    
    (*arbre)->gauche = initNode(infoMem);
    (*arbre)->gauche->mot = "left1";
    (*arbre)->gauche->occ = 3;
    
    (*arbre)->droite = initNode(infoMem);
    (*arbre)->droite->mot = "right1";
    (*arbre)->droite->occ = 7;
    
    (*arbre)->gauche->gauche = initNode(infoMem);
    (*arbre)->gauche->gauche->mot = "left2";
    (*arbre)->gauche->gauche->occ = 1;
    
    (*arbre)->gauche->droite = initNode(infoMem);
    (*arbre)->gauche->droite->mot = "leftright";
    (*arbre)->gauche->droite->occ = 4;
    
    (*arbre)->droite->gauche = initNode(infoMem);
    (*arbre)->droite->gauche->mot = "rightleft";
    (*arbre)->droite->gauche->occ = 6;
    
    (*arbre)->droite->droite = initNode(infoMem);
    (*arbre)->droite->droite->mot = "right2";
    (*arbre)->droite->droite->occ = 9;
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

    if (arbre->gauche) {
        printf("    child {");
        dispTreeRec(arbre->gauche, maxDepth, currentDepth+1);
        printf("}");
    }

    if (arbre->droite) {
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