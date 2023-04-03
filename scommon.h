#ifndef _SCOMMON_H
#define _SCOMMON_H

#ifndef CUSTOM_ALLOC

#define SMALLOC(s) malloc((s))
#define SREALLOC(ptr, s) realloc((ptr), (s))
#define SFREE(ptr) free((ptr))

#endif

#endif