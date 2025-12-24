#include "includes/adt.h"
#include <string.h>
#include "../util/includes/gererMem.h"


Tree initNode(InfoMem * infoMem) {
    Tree ptr = (Tree) myMalloc(sizeof(Node), infoMem);
    ptr->droite = NULL;
    ptr->gauche = NULL;
    ptr->parent = NULL;
    ptr->calque = NULL;
    ptr->occ = 1;
    return ptr;
}


void saisonLegacy(Tree * arbre, Tree t) 
{
    // remettre la REL ici
    while (t->parent && t->occ > (t->parent->occ)) {

        printf("\\chbox{\n");
        printf("Saison : \n");
        printf("\\textbf{« %s » (%d)} ", t->mot, t->occ);
        printf("$\\leftrightarrow$ \\textbf{ « %s » (%d)}\n\n", t->parent->mot, t->parent->occ);
        printf("\n}");

        Tree p = t->parent;

        // parenté <-> parent
        t->parent = p->parent;

        // lien avec le grand parent
        if (p->parent) {
            if (p->parent->droite == p) {
                p->parent->droite = t;
            } else {
                p->parent->gauche = t;
            }
        } else {
            *arbre = t;
        }

        // on insere le parent en fils

        p->parent = t;

        // enfants par "extension"

        int placeGauche = strcmp(t->mot, p->mot) < 0;
        Tree tmp;
        if (placeGauche) {
            tmp = t->gauche;
            t->gauche = p;
        } else {
            tmp = t->droite;
            t->droite = p;
        }

        // on insere fils de t dans ex-parent
        int etaitGauche = p->droite == t;
        if (!etaitGauche) {
            p->gauche = tmp;
            if (tmp) {
                tmp->parent = p;
            }
        } else {
            p->droite = tmp;
            if (tmp) {
                tmp->parent = p;
            }
        }

        // on met à jour les calques
        t->calque = p->mot;
        p->mot = t->calque;

    }
}


void saison(Tree * arbre, Tree t) {
    while (t->parent && t->occ > t->parent->occ) {
        // swap mot
        char *tmpMot = t->mot;
        t->mot = t->parent->mot;
        t->parent->mot = tmpMot;
        
        // swap occ
        int tmpOcc = t->occ;
        t->occ = t->parent->occ;
        t->parent->occ = tmpOcc;
        
        // swap calque
        char *tmpCalque = t->calque ? t->calque : t->mot;
        t->calque = t->parent->calque ? t->parent->calque : t->parent->mot;
        t->parent->calque = tmpCalque;
        
        t = t->parent;
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
        // calque pris en compte que pour le cas d'egalité
        int res_norm = strcmp((*t)->mot, mot);
        int res_calc = (*t)->calque ? strcmp((*t)->calque, mot) : NULL;
        int res = res_norm == 0 ? res_norm : ((*t)->calque ? (
            res_calc == 0 ? res_norm : res_calc
        ) : res_norm);
        par = (Node *) *t; 
        if (res == 0) {
            (*t)->occ++;

            // faire une saison
            dispTree(*arbre, 30, mot);
            saison(arbre, *t);
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

    printf("[");
    if (strcmp(arbre->mot, mot) == 0) {
        printf("fill=yellow");
    }
    printf(", scale=%.2f", 1.0f - ((float)currentDepth / (float)maxDepth));
    printf("]\n");

    printf("{($\\underset{\\text{%s}}{\\text{%s}}$, %d)} \n", 
        arbre->calque ? arbre->calque : "", 
        arbre->mot, 
        arbre->occ
    );

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
    printf("    level 1/.style={sibling distance=75mm, scale=1},\n");
    printf("    level/.style={sibling distance=35mm, scale=.95},\n");
    printf("]\n");

    dispTreeRec(arbre, maxDepth, 0, mot);

    printf(";\n\\end{tikzpicture}\n");
    printf("}\n");
    printf("\\end{center}\n");
}