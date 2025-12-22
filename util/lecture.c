#include <stdio.h>

void lecture(FILE * fichier, void (*func)(char *)) {
    int word_count = 0;
    int word_length = 0;
    char buffer[256];
    char c;

    while ((c = fgetc(fichier)) != EOF) {
        word_length++;
        buffer[word_length] = c;
        if (c == '\n' || c == '\t' || c == ' ') {
            if (word_length > 0) {
                word_count++;
                printf("%s", buffer);
            }
            word_length = 0;
        }
    }
}


FILE * lecture_fichier(char * path) {
    FILE * fichier = fopen(path, "r");
    if (!fichier) {
        fprintf(stderr, "Erreur de lecture du fichier!\n");
        return NULL;
    }
    return fichier;
}