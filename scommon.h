#ifndef _SCOMMON_H
#define _SCOMMON_H

#include <stdint.h>
#include <stdbool.h>

#ifndef CUSTOM_ALLOC

#include <stdlib.h>

#define SMALLOC(s) malloc((s))
#define SREALLOC(ptr, s) realloc((ptr), (s))
#define SFREE(ptr) free((ptr))

#endif

#endif