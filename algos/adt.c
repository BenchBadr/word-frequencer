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


void saison(Tree * arbre, Tree * t) 
{
    if ((*t)->parent && (*t)->occ > 2 * ((*t)->parent->occ)) {

        printf("\\chbox{\n");
        printf("Il faut échanger : \n");
        printf("\\textbf{« %s » (%d)} ", (*t)->mot, (*t)->occ);
        printf("$\\leftrightarrow$ \\textbf{ « %s » (%d)}\n\n", (*t)->parent->mot, (*t)->parent->occ);
        printf("\n}");

        Tree p = (*t)->parent;

        // parenté <-> parent
        (*t)->parent = p->parent;

        // lien avec le grand parent
        if (p->parent) {
            if (p->parent->droite == p) {
                p->parent->droite = *t;
            } else {
                p->parent->gauche = *t;
            }
        }

        // on insere le parent en fils
        if (strcmp(p->mot, (*t)->mot) > 0) {
            Tree tmp = (*t)->droite;
            (*t)->droite = p; // .. un peu un chasles
            p->droite = tmp;

            // on change les parents des enfants
            if (tmp) {
                p->droite->parent = p;
            }
        } else {
            // meme chose a gauche
            Tree tmp = (*t)->droite;
            (*t)->gauche = p;
            p->gauche = tmp;

            if (tmp) {
                p->gauche->parent = p;
            }
        }
        p->parent = *t;
    }
}


void addToTree(InfoMem * infoMem, Tree * arbre, char * mot) {
    // printf("Mot : %s\n", mot);
    Tree * t = arbre;
    Node * par = NULL; // pour stocker le parent

    printf("\\section{« %s »}\n", mot);
    
    printf("\\begin{tcolorbox}[arc=5pt, colback=white!0, colframe=orange!50!black]\n");
    printf("\\infbox{\n");
    printf("Insertion de \\textbf{« %s »}", mot);
    printf("\n}\n");


    while (*t) {
        int res = strcmp((*t)->mot, mot);
        par = (Node *) *t; 
        if (res == 0) {
            (*t)->occ++;

            // faire une saison
            dispTree(*arbre, 30, mot);
            saison(arbre, t);
            printf("\n\\end{tcolorbox}\n");

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
    dispTree(*arbre, 30, mot);
    printf("\n\\end{tcolorbox}");
}


void dispTreeRec(Tree arbre, int maxDepth, int currentDepth, char * mot) {
    if (!arbre || currentDepth >= maxDepth) {
        return;
    }
    
    if (currentDepth == 0) {
        printf("\\node ");
    } else {
        printf("node");
    }

    printf("{%s (%s, %d)} \n", strcmp(arbre->mot, mot) == 0 ? "\\bfseries": "",arbre->mot, arbre->occ);

    if (arbre->gauche && currentDepth+1 < maxDepth) {
        printf("    child {");
        dispTreeRec(arbre->gauche, maxDepth, currentDepth+1, mot);
        printf("}");
    }

    if (arbre->droite && currentDepth+1 < maxDepth) {
        printf("    child {");
        dispTreeRec(arbre->droite, maxDepth, currentDepth+1, mot);
        printf("}");
    }
}

void dispTree(Tree arbre, int maxDepth, char * mot){
    printf("\\begin{center}\n");
    printf("\\tbox{\n");
    printf("\\begin{tikzpicture}\n");
    printf("[\n");
    printf("    level 1/.style={sibling distance=75mm},\n");
    printf("    level 2/.style={sibling distance=35mm},\n");
    printf("]\n");

    dispTreeRec(arbre, maxDepth, 0, mot);

    printf(";\n\\end{tikzpicture}\n");
    printf("}\n");
    printf("\\end{center}\n");
}