#ifndef _SPARSER_H
#define _SPARSER_H

#include "scommon.h"
#include "svec.h"

typedef enum _ParseTokType
{
    PARSE_TOK_PRINT,
    PARSE_TOK_STRING,
} ParseTokType;

typedef struct _ParseTok
{
    ParseTokType type;
    union
    {
        void *value;
        struct _ParseTok *unary;
        struct
        {
            struct _ParseTok *lVal;
            struct _ParseTok *rVal;
        } binary;
    } meta;
} ParseTok;

typedef enum _ParseErr
{
    PARSE_OK,
    PARSE_FAILED,
} ParseErr;

ParseErr parse(SVec *v, SVec *lTokens);

#endif