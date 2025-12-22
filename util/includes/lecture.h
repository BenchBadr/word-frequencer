#ifndef LECTURE
#define LECTURE

#include <stdio.h>

void lecture(FILE * fichier, void (*func)(char *));
FILE * lecture_fichier(char * path);

#endif