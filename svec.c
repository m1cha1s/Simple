#include <stdio.h>
#include <stdlib.h>

#include "svec.h"
#include "scommon.h"

void svecInit(SVec *v)
{
    v->capacity = SVEC_INIT_CAPACITY;
    v->total = 0;
    v->data = SMALLOC(sizeof(void *) * v->capacity);
}

static void svecResize(SVec *v, unsigned int cap)
{
    void **data = SREALLOC(v->data, sizeof(void *) * cap);
    if (data)
    {
        v->data = data;
        v->capacity = cap;
    }
}

void svecAdd(SVec *v, void *data)
{
    if (v->capacity >= v->total)
        svecResize(v, v->capacity * 2);
    v->data[v->total++] = data;
}

void svecSet(SVec *v, unsigned int idx, void *data)
{
    if (idx >= 0 && idx < v->total)
        v->data[idx] = data;
}

void *svecGet(SVec *v, unsigned int idx)
{
    if (idx >= 0 && idx < v->total)
        return v->data[idx];
    return NULL;
}

void *svecDelete(SVec *v, unsigned int idx)
{
    if (idx < 0 || idx >= v->total)
        return NULL;

    void *deleted = v->data[idx];
    v->data[idx] = NULL;

    for (unsigned int i = idx; i < v->total - 1; i++)
    {
        v->data[i] = v->data[i + 1];
        v->data[i + 1] = NULL;
    }

    v->total--;

    if (v->total > 0 && v->total == v->capacity / 4)
        svecResize(v, v->capacity / 2);
}

void svecFree(SVec *v)
{
    SFREE(v->data);
}