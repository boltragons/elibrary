#include "strman.h"

#include <string.h>

String strman_copy(String *destiny, String source) {
    if(!source) {
        return NULL;
    }
    *destiny = (String) realloc(*destiny, strlen(source) + 1);
    strcpy(*destiny, source);
    return *destiny;
}
