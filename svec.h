#ifndef _SVEC_H
#define _SVEC_H

#define SVEC_INIT_CAPACITY 8

typedef struct _SVec
{
    void **data;
    unsigned int capacity;
    unsigned int total;
} SVec;

void svecInit(SVec *);
static void svecResize(SVec *, unsigned int);
void svecAdd(SVec *, void *);
void svecSet(SVec *, unsigned int, void *);
void *svecGet(SVec *, unsigned int);
void *svecDelete(SVec *, unsigned int);
void svecFree(SVec *);

#endif