#include <stdlib.h>
#include <string.h>

#include "../include/utilidades.h"

char * copiarString(const char * string) {
    char * copia = (char*) malloc((strlen(string) + 1) * sizeof(char));

    strcpy(copia, string);

    return copia;
}