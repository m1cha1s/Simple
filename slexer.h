#ifndef _SLEXER_H
#define _SLEXER_H

#include "svec.h"
#include "scommon.h"

typedef enum _LexTokType
{
    LEX_TOK_NEWLINE,

    LEX_TOK_PRINT,
    LEX_TOK_GOTO,

    LEX_TOK_EQ,
    LEX_TOK_PLUS,

    LEX_TOK_ATOM,

    LEX_TOK_INT,
    LEX_TOK_STRING,
} LexTokType;

typedef struct _LexTok
{
    LexTokType type;
    void *meta;
} LexTok;

typedef enum _LexErr
{
    LEX_OK,
    LEX_UNKNOWN_TOKEN,
} LexErr;

LexErr lex(SVec *, char *);

#endif