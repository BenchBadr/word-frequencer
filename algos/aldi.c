
#include <stdio.h>
#include <string.h>
#include "../util/includes/gererMem.h"
#include "includes/aldi.h"



// Distance naif - À remplacer par Levenstein ultérieurement
int distance(const char *s1, const char *s2){
    return (s1[0] > s2[0]) ? (s1[0] - s2[0]) : (s2[0] - s1[0]);
}


Cellule * initCellule(InfoMem * infoMem) {
    Liste ptr = (Cellule *) myMalloc(sizeof(Cellule), infoMem);
    ptr->occ = 1;
    ptr->mot = NULL;
    ptr->suiv = ptr;
    ptr->preced = ptr;
    return ptr;
}

void drawListe(Liste liste) {
    printf("\\circliste{");
    int started = 0;
    for (Cellule * c = liste; c && (!started || c != liste); c = c->suiv) {
        started = 1;
        printf("$\\underset{%d}{\\text{%s}}$", c->occ, c->mot);
        if (c->suiv && c->suiv != liste) {
            printf(",");
        }
    }
    printf("}");
}

void drawListeLin(Liste liste, float scale) {
    printf("\\begin{tcolorbox}[arc=5pt, colback=white!0,center, colframe=orange!50!black]\\begin{center}\n");
    printf("\\scalebox{%.2f}{", scale);
    printf("\\linliste{");
    int started = 0;
    for (Cellule * c = liste; c && (!started || c != liste); c = c->suiv) {
        started = 1;
        printf("$\\underset{%d}{\\text{%s}}$", c->occ, c->mot);
        if (c->suiv && c->suiv != liste) {
            printf(",");
        }
    }
    printf("}}");
    printf("\n\\end{center}\\end{tcolorbox}\n");
}

void addToListe(InfoMem * infoMem, Liste * liste, char * mot, int latex) {
    if (latex) {
        printf("\\section{« %s »}\n", mot);
        printf("\\begin{tcolorbox}[arc=5pt, colback=white!0, colframe=orange!50!black]\\begin{center}\n");
    }

    // 1. Gérer le cas liste inexistante
    if (!(*liste)) {
        *liste = initCellule(infoMem);
        (*liste)->mot = myMalloc(sizeof(char) * strlen(mot) + 1, infoMem);
        strcpy((*liste)->mot, mot);
        (*liste)->suiv = (*liste);
        (*liste)->preced = (*liste);

        if (latex) {
            printf("\\infbox{Init: \\textbf{« %s »}} \n", mot);
            drawListe(*liste);
            printf("\n\\end{center}\\end{tcolorbox}\n\\newpage\n");
        }
        return;
    }

    // 2. Cas de creation aux bornes

    // 2.1 : plus petit que la tete -> mettre a jour tete + l'adresse initiale

    if (strcmp(mot, (*liste)->mot) < 0) {
        if (latex) printf("\\chbox{Création de nœud en tête (avant « %s »)} \n", (*liste)->mot);
        
        Cellule * nptr = initCellule(infoMem);
        nptr->mot = myMalloc(sizeof(char) * strlen(mot) + 1, infoMem);
        strcpy(nptr->mot, mot);

        Cellule * tail = (*liste)->preced;
        nptr->suiv = *liste;
        nptr->preced = tail;
        tail->suiv = nptr;
        (*liste)->preced = nptr;
        
        *liste = nptr; 
        if (latex) { 
            drawListe(*liste); 
            printf("\n\\end{center}\\end{tcolorbox}\n\\newpage\n"); 
        }
        return;
    }

    Cellule * fin = (*liste)->preced;

    // 2.2 : plus grand que la fin -> creer un noeud a la fin
    if (strcmp(mot, fin->mot) > 0) {
        if (latex) printf("\\chbox{Création de nœud en fin (après « %s »)} \n", fin->mot);
        
        Cellule * nptr = initCellule(infoMem);
        nptr->mot = myMalloc(sizeof(char) * strlen(mot) + 1, infoMem);
        strcpy(nptr->mot, mot);

        nptr->suiv = *liste;
        nptr->preced = fin;
        fin->suiv = nptr;
        (*liste)->preced = nptr;
        
        if (latex) { 
            drawListe(*liste); 
            printf("\n\\end{center}\\end{tcolorbox}\n\\newpage\n"); 
        }
        return;
    }

    // 3. Le parcours
    /*
        On a préalablement géré les cas limites (les cas de création)
        On sait donc pour sûr que: Tête ≤ Mot ≤ Fin 
        On cherche donc 'c' tq c < mot < c->suiv.
    */
    
    // determinons la distance pour ensuite choisir (cf. rapport)
    int d_tete = distance(mot, (*liste)->mot);
    int d_fin = distance(fin->mot, mot);
    
    // 1 = sens horaire, 
    // -1 = sens anti horaire
    int direction = (d_tete <= d_fin) ? 1 : -1;

    Cellule * c;

    if (direction == 1) {
        // Recherche en sens direct (sens horaire)
        c = *liste;
        if(latex) printf("\\infbox{$(\\rightarrow)$ Recherche sens horaire (depuis %s)}\n", c->mot);
        
        while (c->suiv != *liste && strcmp(c->suiv->mot, mot) <= 0) {
            c = c->suiv;
        }
    } else {
        // Recherche en sens indrect (anti-horaire)

        c = fin;
        if(latex) printf("\\infbox{$(\\leftarrow)$ Recherche sens Anti-Horaire (depuis %s)}\n", c->mot);
        
        while (c != *liste && strcmp(c->mot, mot) > 0) {
            c = c->preced;
        }
    }

    // 4. Insertion
    /*
        'c' est le bon emplacement a priori
        3 options s'offrent a nous
        1. c = mot, on incremente
        2. c != mot -> insertion après
            - Par ce qui precede
            - c != mot, c < mot et c n'est pas le dernier noeud
            - il faut donc inserer entre c et c->suiv le nouveau noeud pour le mot
            - c'est un cas "d'antisymetrie" donc le mot n'existe necessairement pas deja
            - On parlera d'insertion milieu
    */
    
    // 4.1 - Cas d'egalité
    if (strcmp(c->mot, mot) == 0) {
        if (latex) printf("\\infbox{Incrémentation} \n");
        c->occ++;

    // Cas 4.2 - Insertion après
    } else {
        if (latex) printf("\\chbox{Insertion milieu (après « %s »)} \n", c->mot);
        
        Cellule * nptr = initCellule(infoMem);
        nptr->mot = myMalloc(sizeof(char) * strlen(mot) + 1, infoMem);
        strcpy(nptr->mot, mot);

        nptr->suiv = c->suiv;
        nptr->preced = c;
        
        c->suiv->preced = nptr;
        c->suiv = nptr;
    }

    if (latex) {
        drawListe(*liste);
        printf("\n\\end{center}\\end{tcolorbox}\n\\newpage\n");
    };
}




static void split(Liste source, Liste *part1, Liste *part2) {
    Cellule *tortue = source;
    Cellule *lievre = source;

    // le lièvre avance deux fois plus vite que la tortue
    while (lievre->suiv && lievre->suiv->suiv) {
        lievre = lievre->suiv->suiv;
        tortue = tortue->suiv;
    }

    *part1 = source;
    *part2 = tortue->suiv;

    // on coupe en 2 les deux parties
    tortue->suiv = NULL;
    if (*part2) {
        (*part2)->preced = NULL;
    }
}

static Liste fusionne(Liste a, Liste b, int latex) {
    // cas triviaux
    if (!a) return b;
    if (!b) return a;


    Liste resultat = NULL;
    Liste curseur = NULL;

    while (a && b) {
        Liste choix = NULL;

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
            resultat->preced = NULL;
        } else {
            curseur->suiv = choix;
            choix->preced = curseur;
            curseur = choix;
        }
    }

    if (a) {
        curseur->suiv = a;
        a->preced = curseur;
    } else if (b) {
        curseur->suiv = b;
        b->preced = curseur;
    }

    if (latex) {
        printf("\\subsubsection{Fusion}\n");
        drawListeLin(resultat, 1);
    }
    return resultat;
}


static Liste executerTriFusion(Liste liste, int latex) {
    // Cas trivial
    if (!liste || !liste->suiv) {
        return liste;
    }

    Liste a = NULL;
    Liste b = NULL;

    if (latex) {
        printf("\\subsection{Lancement d'une étape}\n");
        printf("\\subsubsection{Séparation}\n");
        printf("\\noindent\n");

        drawListeLin(liste, 1);
    }

    // on casse la liste
    split(liste, &a, &b);

    if (latex) {

        
        printf("\\noindent\\begin{minipage}{0.5\\textwidth}\\centering\n");
        drawListeLin(a, .7);
        printf("\\end{minipage}");
        
        printf("\\begin{minipage}{0.5\\textwidth}\\centering\n");
        drawListeLin(b, .7);
        printf("\\end{minipage}\n\n");
        
        printf("\\vspace{1cm}\n"); // Un peu d'espace avant la suite
    }

    // on trie recursivement chaque moitié
    a = executerTriFusion(a, latex);
    b = executerTriFusion(b, latex);


    // on fusionne
    return fusionne(a, b, latex);
}


void fusionAldi(InfoMem * infoMem, Liste liste, FILE *file, int latex) {


    // 0. Traitement cas trivial.
    if (!file || !liste || liste->suiv == liste) {
        return; 
    }

    if (latex) {
        printf("\\begin{titlepage}\n");
        printf("    \\centering\n\n");
        printf("    \\vspace*{\\fill}\n\n");
        printf("    {\\Huge \\textbf{Trier la structure}}\n\n");
        printf("    \\vspace{1.5cm}\n\n");
        printf("    {\\Large \\textit{Visualiation du tri fusion sur cette structure}}\n\n");
        printf("    \\vspace*{\\fill}\n\n");
        printf("    \\vspace*{1cm}\n");
        printf("\\end{titlepage}\n\n");
    }



    // 1. Linéarisation (fin de la circularité)
    liste->preced->suiv = NULL;
    liste->preced = NULL;

    if (latex) {
        printf("\\section{Première étape : Casser la circularité}\n");
        drawListeLin(liste, 1);
    }


    // 2. Coeur du tri
    if (latex) {
        printf("\\section{Deuxième étape : Tout casser}\n");
    }

    liste = executerTriFusion(liste, latex);

    // 3. Re-circularisation (inutile si ce n'est pour l'affichage mais gratuite avec le parcours)
    Cellule *dernier = liste;
    while (dernier->suiv) {
        dernier = dernier->suiv;
        fprintf(file, "%s %d\n", dernier->mot, dernier->occ); // ecriture du fichier
    }
    dernier->suiv = liste;
    liste->preced = dernier;

    if (latex) {
        printf("\\newpage\\section{Résultat Final recircularisé}\n");

        printf("\\begin{center}\n");
        drawListe(liste);
        printf("\\end{center}");
    }


}