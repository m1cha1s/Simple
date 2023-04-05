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

static void svecResize(SVec *v, uint32_t cap)
{
    void **data = SREALLOC(v->data, sizeof(void *) * cap);
    if (data)
    {
        v->data = data;
        v->capacity = cap;
    }
}

uint32_t svecLen(SVec *v)
{
    return v->total;
}

void svecAdd(SVec *v, void *data)
{
    if (v->capacity >= v->total)
        svecResize(v, v->capacity * 2);
    v->data[v->total++] = data;
}

void *svecPop(SVec *v)
{
    if (v->total == 0)
        return NULL;

    uint32_t idx = v->total - 1;

    void *data = v->data[idx];

    v->data[idx] = NULL;
    v->total--;

    if (v->total > 0 && v->total == v->capacity / 4)
        svecResize(v, v->capacity / 2);

    return data;
}

void svecSet(SVec *v, uint32_t idx, void *data)
{
    if (idx >= 0 && idx < v->total)
        v->data[idx] = data;
}

void *svecGet(SVec *v, uint32_t idx)
{
    if (idx >= 0 && idx < v->total)
        return v->data[idx];
    return NULL;
}

void svecDelete(SVec *v, uint32_t idx)
{
    if (idx < 0 || idx >= v->total)
        return;

    SFREE(v->data[idx]);
    v->data[idx] = NULL;

    for (uint32_t i = idx; i < v->total - 1; i++)
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
    for (uint32_t i = 0; i < v->total; i++)
        SFREE(v->data[i]);
    SFREE(v->data);
}