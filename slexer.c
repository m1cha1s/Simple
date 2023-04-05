#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "slexer.h"
#include "scommon.h"

#define LEX_TRY(func)    \
    if (tok = (func))    \
    {                    \
        svecAdd(v, tok); \
        continue;        \
    }

static LexTok *lexPrint(char *src, uint32_t *i)
{
    LexTok *tok = NULL;
    if (!strncmp(src, "print", 5))
    {
        tok = SMALLOC(sizeof(LexTok));
        tok->meta = NULL;
        tok->type = LEX_TOK_PRINT;

        *i += 4;
    }
    return tok;
}

static LexTok *lexString(char *src, uint32_t *i)
{
    uint32_t j = 1;
    for (; j < strlen(src); j++)
    {
        if (src[j] == '\"')
            break;
    }

    if (j == strlen(src))
        return NULL;

    LexTok *tok = SMALLOC(sizeof(LexTok));
    tok->type = LEX_TOK_STRING;
    tok->meta = SMALLOC(j);
    char *str = (char *)(tok->meta);

    str[j - 1] = '\0';
    strncpy(str, src + 1, j - 1);

    *i += j;

    return tok;
}

LexErr lex(SVec *v, char *src)
{
    LexTok *tok;
    for (uint32_t i = 0; i < strlen(src); i++)
    {
        tok = NULL;
        switch (src[i])
        {
        case '\n':
        {
            LexTok *tok = SMALLOC(sizeof(LexTok));
            tok->meta = NULL;
            tok->type = LEX_TOK_NEWLINE;
            svecAdd(v, tok);
            continue;
        }
        case 'p':
        {
            LEX_TRY(lexPrint(src + i, &i))
        }
        case '\"':
        {
            LEX_TRY(lexString(src + i, &i))
        }
        default:
            continue;
        }
    }

    return LEX_OK;
}
