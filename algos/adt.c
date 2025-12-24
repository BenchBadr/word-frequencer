#include "includes/adt.h"
#include <string.h>
#include "../util/includes/gererMem.h"

#define LATEX_MODE 0



Tree initNode(InfoMem * infoMem) {
    Tree ptr = (Tree) myMalloc(sizeof(Node), infoMem);
    ptr->droite = NULL;
    ptr->gauche = NULL;
    ptr->parent = NULL;
    ptr->calque = NULL;
    ptr->calque_dir = 0;
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
        // t->calque = strdup(p->mot);
        // p->mot = strdup(t->calque);

    }
}


void saison(Tree * arbre, Tree t) {
    // pour tester : naviguer entre les pages des saisons
    int page = 0;
    while (t->parent && t->occ > t->parent->occ) {
        // Affichage
        if (LATEX_MODE) {
            printf("\\chbox{\n");
            printf("Saison : \n");
            printf("\\textbf{« %s » (%d)} ", t->mot, t->occ);
            printf("$\\leftrightarrow$ \\textbf{ « %s » (%d)}\n\n", t->parent->mot, t->parent->occ);
            printf("\n}");
        }

        Tree p = t->parent;

        int etaitGauche = p->droite == t;

        if (t->calque_dir == 0) etaitGauche ? -1 : 1;

        // swap mot
        char *tmpMot = t->mot;
        t->mot = p->mot;
        p->mot = tmpMot;
        
        // swap occ
        int tmpOcc = t->occ;
        t->occ = p->occ;
        p->occ = tmpOcc;
        
        // if (page == 1) {
        //     dispTree(*arbre, 30, "test");
        // }


        page++;
        
        // passons a l'iteration suivante
        t = t->parent;
    }
}

void addToTree(InfoMem * infoMem, Tree * arbre, char * mot) {
    // printf("Mot : %s\n", mot);
    Tree * t = arbre;
    Node * par = NULL; // pour stocker le parent

    if (LATEX_MODE) {
        printf("\\section{« %s »}\n", mot);
        
        printf("\\begin{tcolorbox}[arc=5pt, colback=white!0, colframe=orange!50!black]\n");
        printf("\\infbox{\n");
        printf("Insertion de \\textbf{« %s »}", mot);
        printf("\n}\n");
    }


    while (*t) {
        // calque pris en compte que pour le cas d'egalité
        int res_norm = strcmp((*t)->mot, mot);
        int res_calc = strcmp((*t)->calque, mot);
        int res;
        res = res_norm == 0 ? res_norm : (res_calc != 0 ? (
            res_calc
        ) : (
            // Cas critique
            (*t)->calque_dir
        ));
        par = (Node *) *t; 
        if (res == 0) {
            (*t)->occ++;

            // faire une saison
            if (LATEX_MODE) {
                dispTree(*arbre, 30, mot);
            }
            saison(arbre, *t);
            if (LATEX_MODE) {
                printf("\n\\end{tcolorbox}\n");
            }

            return;
        } else if (res > 0) {
            t = &((*t)->gauche);
        } else {
            t = &((*t)->droite);
        }
    }
    *t = initNode(infoMem);
    (*t)->parent = (Node *) par;

    char *copy = myMalloc((strlen(mot) + 1) * sizeof(char), infoMem);
    strcpy(copy, mot);
    (*t)->mot = copy;
    (*t)->calque = copy;
    if (LATEX_MODE) {
        dispTree(*arbre, 30, mot);
        printf("\n\\end{tcolorbox}");
    }
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
    if (maxDepth > 5) {
        printf(", scale=%.2f", 1.0f - ((float)currentDepth / (float)maxDepth));
    }

    printf("]\n");

    printf("{($\\underset{\\text{%s } %s}{\\text{%s}}$, %d)} \n", 
        arbre->calque && arbre->calque != arbre->mot ? arbre->calque : "", 
        arbre->calque_dir == -1 ? "\\searrow" : arbre->calque_dir == 1 ? "\\swarrow" : "",
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
    printf("    level/.style={sibling distance=45mm, scale=1},\n");
    printf("]\n");

    dispTreeRec(arbre, maxDepth, 0, mot);

    printf(";\n\\end{tikzpicture}\n");
    printf("}\n");
    printf("\\end{center}\n");
}

void writeTreeRec(Tree arbre, FILE *file) {
    if (!arbre) {
        return;
    }
    
    // IMPORTANT : À reecrire
    fprintf(file, "%s %d\n", arbre->mot, arbre->occ);
    writeTreeRec(arbre->gauche, file);
    writeTreeRec(arbre->droite, file);
}


void writeTree(Tree arbre) {
    FILE *file = fopen("out.txt", "w");
    if (!file) {
        return;
    }
    writeTreeRec(arbre, file);
    fclose(file);
}