#ifndef _SVEC_H
#define _SVEC_H

#include "scommon.h"

#define SVEC_INIT_CAPACITY 8

typedef struct _SVec
{
    void **data;
    uint32_t capacity;
    uint32_t total;
} SVec;

void svecInit(SVec *);
static void svecResize(SVec *, uint32_t);
uint32_t svecLen(SVec *);
void svecAdd(SVec *, void *);
void *svecPop(SVec *);
void svecSet(SVec *, uint32_t, void *);
void *svecGet(SVec *, uint32_t);
void svecDelete(SVec *, uint32_t);
void svecFree(SVec *);

#endif