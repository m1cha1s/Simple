#ifndef _SVALUE_H
#define _SVALUE_H

typedef enum _SValueType
{
    SVALUE_STRING,
    SVALUE_INT,
} SValueType;

typedef struct _SValue
{
    SValueType type;
    void *val;
} SValue;

#define SVALUE_IS_STRING(val) ((val)->type == SVALUE_STRING)
#define SVALUE_IS_INT(val) ((val)->type == SVALUE_INT)

#define SVALUE_AS_STRING(val) (char *)((val)->val)
#define SVALUE_AS_INT(val) (int)(*((val)->val))

#endif