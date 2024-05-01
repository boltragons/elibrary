#ifndef STRMAN_H
#define STRMAN_H

#include "common.h"

/* Public Functions */

/**
 * Copies a string to a String object (dinamically allocated).
 */
String strman_copy(String *destiny, String source);

#endif
