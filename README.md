$\newcommand{\hsp}{\hspace{3em}}$

# Préliminaires

$\hsp$ Quand on rédige un document, il n'est pas rare que l'on ait une fonctionnalité d'auto-complétion, reposant très souvent sur les chaînes de Markov, impliquant préalablement une analyse fréquentielle des mots voire de leurs relations de successions. Disposer de la fréquence de mots dans un langage donné, un contexte donné et selon une période donnée peut se révéler grandement utile ne serait-ce que pour envisager d'imiter ce même style. 
C'est ainsi une tâche dont on usera et abusera pour différents but et de ces innombrables exécutions pouvoir économiser un lapsus de temps se révélerait être une économie de temps et d'énergie considérable. 
Dans ce cadre, il est plus que nécessaire de pouvoir effectuer cette tâche de la façon la plus optimale possible, c'est ainsi que nous nous y essayerons selon différentes approches gardant à l'esprit la question suivante : **Jusqu'où pouvons-nous descendre ?**
Nous détaillerons d'abord dans cette phase préliminaire la démarche adoptée et les méthodes employées pour la mise en place de benchmarks. S'en suivra le cœur du projet et les détails techniques sur les algorithmes, finalement une troisième partie portant sur des utilisations concrètes de ces algorithmes avec des exemples à l'appui.


## Structure du projet

- `algos/`
	-  `includes/` : fichier headers associés
	- `adt.c` : implémentation de l'arbre dynamique trié (ADT) décrit dans ce rapport
- `data/` : contient les livres de test en fichier texte
	- `ubu-roi.txt` : Ubu Roi, Alfred Jarry. Source : [Projet Gutenberg](https://www.gutenberg.org/cache/epub/16884/pg16884.txt)
- `examples/`
	- `chart.tex` : Exemple de création de graphique
- `util/`
	- `includes/` : fichier headers associés
	- `gererMem.c` : décrite dans le sujet
	- `lecture.c` : permet la lecture successive des mots
- `main.c` : fichier principal
- `Makefile` : simplifiant grandement les commandes


## Mise en place des outils fondamentaux

### Outil de lecture

$\hsp$À la manière de `qsort`, on crée dans `util/lecture.c` une fonction `lecture` prenant en argument une fonction de gestion des mots qui sera utilisée ultérieurement. 
On fait pour cela quelques choix:
- éliminer tous les mots contenant un caractère spécial car ils résultent souvent d'une mauvaise transcription (eg. `FIN.nais!`).
- tout mettre en minuscule à l'exécution.
- on supprime les accents afin de simplifier les comparaisons

## Schémas

Tous les schémas qui vont suivre ont été produit directement pendant l'exécution sous forme de balise de code TikZ. Les fonctions utilisées pour les affichages ainsi que leur emplacement seront explicitées en dessous des schémas (sans redondance).


## Manuel de l'utilisateur

Le programme dispose d'un `Makefile` simplifiant la compilation.

Pour compiler : `make`
Pour exécuter : `main`

Ci-dessous les options dont vous disposez. 

```
Options (and corresponding environment variables):
-a       : Algorithm choice. Please refer to `rapport.pdf` to learn more about each. 
           Valid options : `bogus`, `aldi`, `adt`, `hash`
           Default : `adt`
-out     : Specify produced text output file. Default : `sorties/out.txt`
        .
-l       : Latex mode. Produces debugging utility written in `examples/tests/out.tex`.
           Default : unset
-g       : Create a graph of the execution (such as shown in `examples/chart.tex`). Will return both curve for time evolution and memory usage evolution.
-ml      : Specify the minimum of letters per word to be considered. (default : 1)
-help    : Learn more about the options.
Files    : Specify path of the input files. Max : 5
           Default : `data/hittites.txt`
```

## Bilan de correction

**Améliorations**
- $(\star)$ Une option permettant à l’utilisateur de ne demander que les mots d’au moins $k$ lettres (où $k$ est spécifié par l’utilisateur)
- $(\star \star)$ Implémenter quatre algorithmes ou plus

**Amélioration suggérées**
- $(?)$ Création de visuels interactifs pour aider à l'assimilation du fonctionnement des programmes avec \\LaTeX et TikZ. 

---

# Les algorithmes et leur fonctionnement

Dans cette section nous noterons:
- l'ensemble des mots de taille inférieure ou égal à $n$ : $\mathbb M^n$ 
- $f_i:\mathbb{M}^n \to  \mathbb R$ la  fonction associant à un mot sa fréquence dans un corpus donné à l'itération $i$.
- Nous noterons $m_k$  pour $k \in \{1, \dots, n\}$ l'ensemble de mots distincts d'un corpus donné.
- Et on considère $(a,b) \in (1, \dots, n)\times (1, \dots, n)$ 
Chaque algorithme sera présenté selon les sous-sections suivantes:
- Description de l'approche
- Rendement (statistiques et courbes voire, si possible une estimation de la complexité)
- Problèmes de l'approche
- On utilisera systématiquement la fonction `lecture` afin d'insérer, on admet son utilisation pour tous les algorithmes

On introduit également la notion de distance entre les mots, notée $d(m_a, m_b)$
- On choisit pour cela d'utiliser la **distance de Levenstein**, 
- en effet cette dernière permet des longueurs différentes en prenant en compte les insertions et suppressions en plus des substitutions
- contrairement à la **distance de Hamming** qui elle se limite aux substitutions bien que les deux soient très similaires.

---
## Allocation dynamique et tri à bulles (BOGUS)

### Description de l'approche

**Étape 1** - *Lecture et construction de la zone de mémoire associée*

$\hsp$On se propose ici d'utiliser une allocation dynamique afin de stocker nos clés (structure associée). On veut minimiser l'espace perdue de façon sûre, on suppose pour cela que le ratio maximal entre le nombre de mots et le nombre de caractères sera de $\frac12$ pour un fichier où tous les mots font un caractère, en effet, la moitié (ou presque) des caractères décrit des séparations.

La lecture de la taille du fichier (en octets) est constante. Selon les caractères utilisés un caractère vaut entre 1 et 4 octets (selon s'il est dans l'ASCII, ASCII étendu, UTF-8 ou encore UTF-16...).
Donc la mesure sera toujours effective s'il y a des caractères spéciaux (plus que 1 octet) et résultera d'une allocation légèrement plus généreuse. 

**Étape 2** - *Tri à bulles pour la remontée*

$\hsp$L'ordre des mots dans cette dernière ne sera que chronologique (selon leur ordre d'insertion) afin d'éviter de permuter ou insérer ce qui peut dans certains cas être coûteux (s'il faut déplacer tous les éléments qui suivent par exemple). 

On trie seulement à la fin en faisant « remonter » les mots avec le plus d'occurrences à mesure que l'on parcourt la liste. Pas de problème de décalage cette fois car il ne s'agira que d'échanges d'adresses et non d'insertions à proprement parler. 

Enfin, un parcours de cette structure permettra de produire le document souhaité.


### Rendement

*Insérer courbe et stats*


---
## Avec liste doublement chaînées circulaires et tri par insertion (ALDI)

### Description de l'approche

$\hsp$On lit successivement tous les mots et on les insère selon l'ordre lexicographique. L'utilisation d'une liste ou d'une allocation dynamique pour allouer toute la zone de stockage se révèle peu efficace pour deux raisons:
1. Taille non connue $\to$ risque de multiplication des réallocations (coûteuses) ou, alternativement trop d'espace perdu la zone initiale étant trop grande.
2. Insertions complexes $\to$ Insérer à la $i$-ème position implique de déplacer tous les mots suivants et laisser des mitages implique de nouveau de l'éventuel espace perdu.

**Étape 1** - *Choix de la structure*

```tikz
\begin{document}
\begin{tikzpicture}
\tikzset{
  person/.pic={
    \draw (0,0) circle (1mm);
    \draw (0,-1mm) -- (0,-5mm);
    \draw (-2mm,-2.5mm) -- (2mm,-2.5mm);
    \draw (0,-5mm) -- (-2mm,-7mm);
    \draw (0,-5mm) -- (2mm,-7mm);
  }
}


\pic at (0,0) {person};

\draw[fill=red] (-2, 1.8) circle (1mm);

\draw[>-<]
  (-5mm,-5mm)
  .. controls (-6,4) and (6,4) ..
  (5mm,-5mm);
\end{tikzpicture}

\end{document}
```

Vaut-il mieux se diriger à droite ou à gauche pour capturer le petit cercle rouge ?
C'est à cette question que s'apparente l'insertion des mots selon leur ordre lexicographique, on choisit selon la distance entre les mots (notion élaborée plus haut) s'il faut aller à gauche ou à droite.
Il s'agira désormais de spécifier l'usage de cette structure. 
- par exemple
	- $d(\text{'A'}, \text{'W'}) = 22$ 
	- $d(\text{'Z'}, \text{'W'}) = 3$ 
- c'est un choix qui se fera au début du parcours, s'apparentant à la question suivante:
	- *Il vaut mieux le chercher en partant de la fin, se déplaçant vers la gauche ou du début en s'orientant vers la droite ?*
- d'où l'utilisation d'une liste doublement chaînée et circulaire. 

---

**Étape 2** - *Remplissage de la structure*

On utilise un tri fusion afin de réorganiser notre liste chaînée selon les occurrences plutôt que l'ordre lexicographique. 

Enfin, on parcours de bout en bout notre structure réorganisée pour produire le fichier final (classé par ordre décroissant des occurrences).
Voici un extrait du PDF retraçant l'exécution (extrait complet disponible dans `examples/aldi.pdf`) permettant de visualiser la structure.  

```tikz
\begin{tcolorbox}[arc=5pt, colback=white!0, colframe=orange!50!black]\begin{center}
\infbox{$(\leftarrow)$ Recherche sens Anti-Horaire (depuis with)}
\chbox{Insertion milieu (après « middle »)} 
\circliste{$\underset{2}{\text{and}}$,$\underset{1}{\text{assyrian}}$,$\underset{1}{\text{bc}}$,$\underset{1}{\text{between}}$,$\underset{1}{\text{centuries}}$,$\underset{1}{\text{coming}}$,$\underset{1}{\text{conflict}}$,$\underset{1}{\text{dominant}}$,$\underset{1}{\text{east}}$,$\underset{1}{\text{egypt}}$,$\underset{2}{\text{empire}}$,$\underset{1}{\text{hittites}}$,$\underset{1}{\text{into}}$,$\underset{1}{\text{kingdom}}$,$\underset{1}{\text{middle}}$,$\underset{1}{\text{mitanni}}$,$\underset{1}{\text{near}}$,$\underset{1}{\text{new}}$,$\underset{4}{\text{of}}$,$\underset{1}{\text{one}}$,$\underset{1}{\text{powers}}$,$\underset{7}{\text{the}}$,$\underset{1}{\text{were}}$,$\underset{1}{\text{with}}$}
\end{center}\end{tcolorbox}
```

Voici les différentes étapes de l'algorithme:
1. Gérer le cas liste inexistante
2. Cas de creation : il faut créer une cellule aux bornes
	- 2.1. plus petit que la tete $\to$ mettre a jour tete + l'adresse initiale
	- 2.2. plus grand que la fin $\to$ créer un noeud a la fin
3. Le parcours
	- On a préalablement géré les cas limites (les cas de création)
	- On sait donc pour sûr que: Tête $≤$ Mot $≤$ Fin
	- On cherche donc `c` tq `c` < `mot` < `c->suiv`.
	- déterminons la distance pour ensuite choisir (cf. rapport)
	- 1 = sens horaire,
	- -1 = sens anti horaire
	- Recherche dans le sens choisi (donc avec des suivant ou précédant respectivement)
4. Insertion
	- Par ce qui precede
		- `c` $\neq$ `mot`, `c` < `mot` et `c` n'est pas le dernier noeud
		- il faut donc insérer entre `c` et `c->suiv` le nouveau nœud pour le mot
		- c'est un cas "d'antisymétrie" donc le mot n'existe nécessairement pas deja
		- On parlera d'insertion milieu 
---
### Tri fusion

Le choix du tri fusion pour cet algorithme semble assez évident. En effet, la structure s'y porte à perfection au vu du fait qu'on peut débrancher et rebrancher les nœuds, nous affranchissant de toute nécessité de stocker des données ailleurs.

**En première étape**, on casse la circularité de la structure. À noter que ce visuel est dynamique obtenu avec l'usage de congruences. 
- Pour $n$ le nombre de nœuds à afficher pour une liste et $i$ l'index du nœud
	- Position en $x$ : $\dfrac{1 + (-1)^{\lfloor i/n \rfloor}}2 \times n + (-1)^{\lfloor i/n \rfloor} \times (i \mod n)$      
		- Permet de créer des séquences du type: (exemple avec $n = 4$)
		- $\{4,3,2,1\},\{1,2,3,4\},\{4,3,2,1\},...$
	- Position en $y$ : $(-1) \times \lfloor i/ n \rfloor$ (l'axe vertical est inversé avec pgfmath, donc dans le sens des maths plutôt que le sens numérique)
```tikz
\begin{tcolorbox}[arc=5pt, colback=white!0,center, colframe=orange!50!black]\begin{center}

\scalebox{1.00}{\linliste{$\underset{7}{\text{the}}$,$\underset{4}{\text{of}}$,$\underset{2}{\text{and}}$,$\underset{2}{\text{empire}}$,$\underset{1}{\text{assyrian}}$,$\underset{1}{\text{bc}}$,$\underset{1}{\text{between}}$,$\underset{1}{\text{centuries}}$,$\underset{1}{\text{coming}}$,$\underset{1}{\text{conflict}}$,$\underset{1}{\text{dominant}}$,$\underset{1}{\text{east}}$,$\underset{1}{\text{egypt}}$,$\underset{1}{\text{hittites}}$,$\underset{1}{\text{into}}$,$\underset{1}{\text{kingdom}}$,$\underset{1}{\text{middle}}$,$\underset{1}{\text{mitanni}}$,$\underset{1}{\text{near}}$,$\underset{1}{\text{new}}$,$\underset{1}{\text{one}}$,$\underset{1}{\text{powers}}$,$\underset{1}{\text{were}}$,$\underset{1}{\text{with}}$}}

\end{center}\end{tcolorbox}
```

**En deuxième étape**, on souhaite définir un mécanisme de séparation de la liste. Il faut donc trouver le milieu. 
En l'absence de la notion d'index ou de longueur, on définit deux curseurs. 
- Un curseur rapide (lièvre), qui parcourt 2 nœuds par itération
- Un curseur lent (tortue), qui parcourt 1 nœud par itération
Lorsque le curseur rapide aura atteint la fin de la liste, le curseur lent sera à la moitié. 
On débranche donc la liste chaînée à cette moitié. (voir la fonction `split`).

Le reste du programme relève du tri fusion sans plus de complications.

> [!info]
> **Question** : Était-ce nécessaire de décirculariser ?
> **Réponse** : Non, mais c'était plus agréable visuellement pour débuguer les étapes du tri fusion




---
### Rendement

**Courbe d'évolution du temps d'exécution**

```tikz
\chart{10}{(0,0) (0,0.534500)(1,0.801700)(2,1.243400)(3,1.724300)(4,2.288500)(5,2.905200)(6,3.619200)(7,4.299300)(8,5.001400)(9,5.745900)(10,6.575600)}{Nb. mots (1/859)}{Temps d'ex. (s)}
```

**Courbe d'évolution de l'utilisation de la mémoire**

```tikz
\chart{10}{(0,0) (0,1.000000)(1,1.010668)(2,1.032719)(3,1.066062)(4,1.112702)(5,1.170268)(6,1.243017)(7,1.327437)(8,1.424030)(9,1.526868)(10,1.640041)}{Nb mots (1/859)}{Max alloc (1/65802)}
```

---
## Arbre saisonnier (ADT)

### Description de l'approche

$\hsp$On parlera plus rigoureusement d'arbre dynamiquement trié (ADT), surnommé ici « l'arbre saisonnier » en analogie avec les arbres qui se transforment au fil des saisons tandis que celui ci se transforme au fil des insertions. Il mobilise la notion d'arbre binaire de recherche mais fortement adaptée à son usage.

Un arbre binaire de recherche (ABR) est un arbre binaire construit de telle sorte à ce qu'on puisse le parcourir méthodiquement et permettant des insertions triées naturellement. 
En effet, pour un nœud considéré la droite décrit des éléments supérieurs à ce dernier selon la relation de comparaison fournie tandis que la gauche décrit des élément inférieurs et ce récursivement.
Selon la mise en œuvre de l'ABR on peut interdire ou non les cas d'égalité. En ce qui nous concerne les cas d'égalité seront bien évidemment supprimés avec une incrémentation du compteur d'occurrences en guise de substitut.

Un problème subsiste : un arbre binaire de recherche décrit des comparaisons sans pour autant être trié. Il est très facile de construire un exemple non-trié puisqu'il y a une idée de chronologie d'insertion qui prime.
Ainsi, il est nécessaire de permuter des nœuds quand les occurrences l'obligent. Des contraintes s'imposent à cet effet. 

Un arbre peu se révéler pratique quand trié et un exemple frappant en est le code morse. Ce dernier a été construit selon la fréquences des lettres en anglais de façons à ce que les lettres les plus fréquentes demandent le moins d'étapes (E = ., T = -). 
En adoptant une approche similaire, on veut s'assurer que les insertions de mots demandent moins d'étapes, à fortiori moins de comparaison pour "trouver leur place", ce qui prendra son sens puisque l'accès se produit à chaque ajout d'occurrence. 

**En d'autres termes**, c'est comme si on construisait une table du morse à mesure que l'on parlait selon la fréquence des mots à mesure qu'on les dit. 

Cet arbre est dit dynamique car en tout point si l'occurrence d'un mot augmente beaucoup (comparé aux autres mots) il s'élèvera naturellement dans l'arbre avec pour condition que le mot à remonter soit significativement gagnant.

**Au départ,**
- on souhaite éviter des permutations répétées sur des mots proches :
	- $f_i(m_a) > f_{i}(m_b)$ et  $f_{i+\delta}(m_a) < f_{i+\delta}(m_b)$ 
	- avec $\delta$ petit
- L'approche choisie est la suivante:
	- Permutation si et seulement si:
		- $f_{i+\delta}(m_a) \prec f_{i+\delta}(m_b) \iff 2 \cdot f_{i+\delta}(m_a) < f_{i+\delta}(m_b)$ 
**Problème**
- L'arbre n'était pas complètement trié (du moins verticalement) donc cette approche fut abandonnée au profit d'une comparaison « directe » avec pour condition
	- $f_i(m_a) > f_i(m_b)$
- et $m_b \to m_a$ (parent)

**Bilan**
- Trier d'abord selon l'ordre alphabétique
- En deuxième étape, on remonte les éléments en **cassant l'ordre alphabétique** pour les rendre plus accessibles.

Le résultat final (donc écriture du fichier de sortie) se fera dans le cadre d'un **parcours en profondeur** de ce même arbre à la fin de l'exécution.

### Conception

Voici un exemple avec une phrase plutôt courte en guise d'illustration (extrait de [cet article](https://en.wikipedia.org/wiki/Hittites)).

> [!info]
> Cet arbre est tronqué à une profondeur de 5 pour des raisons de visibilité dans le rapport. 

Jusque-là, rien d'intéressant 

```tikz
\begin{document}
\begin{tikzpicture}
[
    level 1/.style={sibling distance=55mm},
    level 2/.style={sibling distance=35mm},
]
\node  {\small (between, 1)} 
    child {node {\small (and, 2)} 
    child {node {\small (bc, 1)} 
    child {node {\small (assyrian, 1)} 
}}}    child {node {\small (the, 7)} 
    child {node {\small (centuries, 1)} 
    child {node {\small (hittites, 1)} 
    child {node {\small (dominant, 1)} 
}    child {node {\small (one, 1)} 
}}}    child {node {\small (were, 1)} 
    child {node {\small (with, 1)} 
}}};
\end{tikzpicture}
\end{document}
```

*Source du schéma :* `algos/adt.c` $\to$ `void dispTree(Tree arbre, char * mot, int maxDepth)`

**Étape 1** - *Faisons un débuggage...*

Cette tâche était délicate à visualiser. On adapte les fonctions pour générer un fichier \LaTeX contenant la trace de l'algorithme de création de l'arbre, à savoir en affichant TOUTES les étapes  puis en lançant le code avec `> examples/saisons/out.tex`. 
C'est très pratique car en lançant le code, le code \LaTeX est immédiatement compilé, permettant un visuel complet des différents état de l'arbre à la fin de l'exécution, à la manière de logs avec en début le mot à insérer et, s'il y en a ce que la saison va engendrer à la fin.

On remarque un **gros** problème! Puisque l'ordre lexicographique est cassé au bout d'un échange, même un peu le placement d'un élément diffère ce qui casse tout en terme de comptage. 
- Le problème était du au fait qu'une saison permutait simplement la paire $(m_a, (f(m_a)))$ or, il aurait fallu intervertir les enfants et faire une insertion à proprement parler (en débranchant puis rebranchant). 
- Ça marche partiellement mais certains mots apparaissent toujours « divergents » (non-unicité dans l'arbre)
- Il faut donc revoir toute notre façon d'insérer
- Récapitulons,
	1. On sait qu'à l'état $i$ l'arbre est trié selon les occurrences
	2. On se « repère » quand on ne sait pas (occurrences égales) avec l'ordre lexicographique
	3. On ne dispose naturellement pas du nombre d'occurrences d'un mot à insérer même s'il l'a déjà été (donc pas possible d'insérer par occurrence). 
	4. On pense donc à introduire une notion de **calque**
		- Un calque permettra de comparer au prédécesseur (donc avant interversions des saisons).
		- Tout élément placé à l'aide d'une saison dispose d'un calque qu'il s'agisse du parent ou de l'enfant.
		- Imaginez donc que vous avez un bouton qui affiche l'arbre sans saisons quand vous le souhaitez, permettant de passer outre ces interversions pendant les insertions.
		- Cette approche nous conditionne à ne plus conserver les enfants des nœuds respectifs.
- **Cas critique**
	- Et si le calque est égal au mot à insérer?
		- Fortes chances de créer une divergence, 
		- on ne peut pas utiliser l'ordre des calques et l'ordre lexicographique est inutile sans utiliser les calques.
		- *On ne sait pas où aller!* 
		- On crée un nouvel attribut `calque_dir` décrivant la direction dans laquelle s'est effectuée la saison (0 : pas de saison, -1 : monté de la branche droite, 1 monté de la branche gauche)
		- On affiche les directions données par `calque_dir` dans le PDF avec des flèches après le calque.
- **Résultat**
	- Ça marche! 
	- On obtient un arbre trié à l'issue de l'exécution sans divergence!!!



**Étape 2** - *Visualisation du nouvel arbre*

$\hsp$Voici un extrait de profondeur majorée à 4. 
On obtient bien le résultat souhaité avec, comme convenu un accès dynamiquement simplifié aux nœuds les plus fréquents!
Les mots « en dessous » sont les calques associés.
Il vous est possible de voir le PDF de l'algorithme final avec toutes les étapes dans le dossier `examples/` du rendu à titre informatif. 


```tikz
\usepackage{amsmath}
\begin{document}
\begin{tikzpicture}
[
    level 1/.style={sibling distance=75mm, scale=1},
    level/.style={sibling distance=45mm, scale=1},
]
\node []
{($\underset{\text{between}}{\text{the}}$, 7)} 
    child {node[]
{($\underset{\text{}}{\text{and}}$, 2)} 
    child {node[]
{($\underset{\text{}}{\text{bc}}$, 1)} 
    child {node[]
{($\underset{\text{}}{\text{assyrian}}$, 1)} 
}}}    child {node[]
{($\underset{\text{one}}{\text{of}}$, 4)} 
    child {node[]
{($\underset{\text{egypt}}{\text{empire}}$, 2)} 
    child {node[]
{($\underset{\text{the}}{\text{between}}$, 1)} 
}    child {node[]
{($\underset{\text{of}}{\text{centuries}}$, 1)} 
}}    child {node[]
{($\underset{\text{}}{\text{were}}$, 1)} 
    child {node[]
{($\underset{\text{}}{\text{with}}$, 1)} 
}}};
\end{tikzpicture}
\end{document}
```



**Étape 3** - *Génération du fichier de sortie*

*Quel problème dans l'arbre actuel ?*
Faites visuellement un parcours en largeur et il est facile de constater que les mots sont triés par fréquence verticalement seulement mais pas horizontalement. 
Il s'agira donc d'effectuer en dernière étape un tri fusion selon les différentes branches de l'arbre. 

Cette fonction est un parcours en profondeur (itératif) utilisant deux files (FIFO). 
Il est inspiré de la 2ème étape du tri fusion, à savoir la fusion. 

La seule propriété prise pour acquis est que tout parent est plus grand que l'enfant, grâce aux saisons, 
- ainsi, en comparant les deux fils on sait lequel insérer en 1er
- or, rien ne garantit que l'autre fils est inséré directement après, prenons un exemple;
	- 5 $\to 2$ et $5 \to 3$  avec $4 \to 3$ 
	- Un parcours correct serait
		- $5 \to 4 \to 3 \to 2$
	- Il est donc nécessaire de mettre le fils inférieur en attente (n'étant pas descendant direct aucune propriété ne permet d'affirmer quand l'insérer)


---
### Rendement

Schémas générés à l'aide de la fonction `grapher` (disponible dans `main.c`)

**Courbe d'évolution du temps d'exécution**
```tikz
\chart{10}{(0,0) (0,0.303900)(1,0.401000)(2,0.540200)(3,0.718600)(4,0.955600)(5,1.222200)(6,1.517200)(7,1.835100)(8,2.211900)(9,2.690000)(10,3.151900)}{Nb. mots (1/859)}{Temps d'ex. (s)}
```

**Courbe d'évolution de l'utilisation de la mémoire**
```tikz
\chart{10}{(0,0) (0,1.000000)(1,1.385375)(2,1.770749)(3,2.156124)(4,2.541498)(5,2.926873)(6,3.312248)(7,3.697622)(8,4.082997)(9,4.468371)(10,4.853746)}{Nb mots (1/859)}{Max alloc (1/35664)}
```


Il est facile de remarquer que cet algorithme se révèle bien plus efficace en terme de temps d'exécution que l'ALDI.

L'utilisation de la mémoire quant à elle semble aussi mieux gérée, l'unité de représentation du pic d'allocation étant de moitié et la courbe associée est linéaire tandis que pour l'ALDI il s'agit d'une courbe convexe (donc croît nécessairement plus rapidement que pour le cas linéaire).

---
## Hachage et fusion (HASH)

$\hsp$ Après la mise en place de ces trois algorithmes, on constate que ce qui augmente énormément les coûts pour chacun de ces programmes est la recherche des mots étudiés. Pour le cas de l'ADT tout a été pensé pour qu'il soit rendu plus simple.

Dans cette 4ème et dernière approche il sera question de tables de hachage. Nous utiliserons pour se faire la fonction de hachage fournie dans le cours « Algorithmique et structures de données » à savoir le 64-bit FNV-1a pour ensuite effectuer un tri fusion sur les données insérer. 

**Étape 1** - *Choix des structures de données*

On utilise (comme imposé par le hachage externe) un tableau dynamique afin de stocker des listes chaînées (décrivant les éléments de même hash).  
Après une légère hésitation, il est décidé de ne pas faire d'insertion triée dans ces listes chaînées car cela ne permettrait pas pour autant d'effectuer par exemple une dichotomie de façon efficace au vu du fait qu'il faudrait encore utiliser la méthode du lièvre et de la tortue pour trouver le milieu.
On se contente donc d'une liste chaînée simple sans attribut additionnel.  

Afin de gérer l'évolution de la taille on utilise 


---
# Applications pratiques implémentées

## Amélioration mineures

## Classification par thème

## Variante française du morse

## Fenêtre glissante

---

# Références utilisées

- Ubu Roi, Alfred Jarry, version texte
	- https://www.gutenberg.org/cache/epub/16884/pg16884.txt
- Choix de la méthode de calcul de la distance entre deux mots
	- [Distance entre deux mots, Exo7Math, Université de Lille](https://irem.univ-lille.fr/~site/IMG/pdf/142_distance_entre_les_mots.pdf)
- Méthode du lièvre et de la tortue
	- [GFG, Middle in a Linked List](https://www.geeksforgeeks.org/dsa/write-a-c-function-to-print-the-middle-of-the-linked-list/)
- *Cours 5 - Tableaux dynamiques, tables de hachages*,
	- Algorithmique et structures de données (Cyril Nicaud) 