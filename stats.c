void saison(Tree * arbre, Tree node) 
{
    while (node && node->parent && node->occ > node->parent->occ) {

        printf("\\chbox{\n");
        printf("Saison : \n");
        printf("\\textbf{« %s » (%d)} ", node->mot, node->occ);
        printf("$\\leftrightarrow$ \\textbf{ « %s » (%d)}\n\n", node->parent->mot, node->parent->occ);
        printf("\n}");

        Tree p = node->parent;

        // parenté <-> parent
        node->parent = p->parent;

        // lien avec le grand parent
        if (p->parent) {
            if (p->parent->droite == p) {
                p->parent->droite = node;
            } else {
                p->parent->gauche = node;
            }
        } else {
            *arbre = node;
        }

        // on insere le parent en fils
        p->parent = node;

        int placeGauche = strcmp(node->mot, p->mot) < 0;
        Tree tmp;
        if (placeGauche) {
            tmp = node->gauche;
            node->gauche = p;
        } else {
            tmp = node->droite;
            node->droite = p;
        }

        // on insere fils de node dans ex-parent
        int wasRightChild = (p->droite == node);
        if (wasRightChild) {
            p->droite = tmp;
        } else {
            p->gauche = tmp;
        }
        if (tmp) {
            tmp->parent = p;
        }
    }
}