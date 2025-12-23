#ifndef LECTURE
#define LECTURE

#include <stdio.h>
#include "gererMem.h"

void lecture(InfoMem * infoMem, FILE * fichier, void (func)(InfoMem * infoMem, void * adr, char * mot), void * adr);
FILE * lecture_fichier(char * path);
char remove_accent(char c);

#endif