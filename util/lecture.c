#include <stdio.h>
#include "includes/gererMem.h"


void lecture(InfoMem * infoMem, FILE * fichier, void (func)(InfoMem * infoMem, void * adr, char * mot), void * adr) {
    char buffer[128];
    int word_length = 0;
    // int word_count = 0;
    // int total_char = 0;
    int is_a_word = 1;
    char c;

    while ((c = fgetc(fichier)) != EOF) {
        // total_char++;

        if ((c < 'A' && c > 'z') || c == '\n' || c == '\t' || c == ' ' || c == ',') {

            if (is_a_word && word_length > 1) {
                buffer[word_length] = '\0';
                func(infoMem, adr, buffer);
            }

            word_length = 0;
            is_a_word = 1;
            // word_count++;

        } else if ((c < 'A' || c > 'z')) {
            is_a_word = 0;
        } else if (is_a_word) {
            if (c >= 'a') {
                buffer[word_length] = c;
            } else {
                buffer[word_length] = c + 'a' - 'A';
            }
            word_length++;
        }
    }

    // dernier mot
    if (word_length > 1 && is_a_word) {
        word_length++;
        buffer[word_length] = '\0';
        func(infoMem, adr, buffer);
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