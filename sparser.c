#include "sparser.h"
#include "slexer.h"

#define PARSE_TRY(func) \
    if ((tok = (func))) \
        return tok;

ParseTok *parseTok(SVec *lTokens, uint32_t *i);

ParseTok *parseString(SVec *lTokens, uint32_t *i)
{
    LexTok *ltok = (LexTok *)svecGet(lTokens, *i);

    ParseTok *tok = SMALLOC(sizeof(ParseTok));
    tok->type = PARSE_TOK_STRING;
    tok->meta.value = ltok->meta;

    (*i)++;
    return tok;
}

ParseTok *parsePrint(SVec *lTokens, uint32_t *i)
{
    if ((*i) + 1 >= svecLen(lTokens))
        return NULL;
    switch (((LexTok *)svecGet(lTokens, (*i) + 1))->type)
    {
    case LEX_TOK_ATOM:
    case LEX_TOK_STRING:
    case LEX_TOK_INT:
    {
        ParseTok *tok = SMALLOC(sizeof(ParseTok));
        tok->type = PARSE_TOK_PRINT;

        (*i)++;
        tok->meta.unary = parseTok(lTokens, i);
        (*i)++;
        return tok;
    }
    default:
        return NULL;
    }
}

ParseTok *parseTok(SVec *lTokens, uint32_t *i)
{
    ParseTok *tok = NULL;
    if (*i + 1 < svecLen(lTokens))
        switch (((LexTok *)(svecGet(lTokens, *i + 1)))->type)
        {
        default:
            break;
        }
    switch (((LexTok *)(svecGet(lTokens, *i)))->type)
    {
    case LEX_TOK_PRINT:
        PARSE_TRY(parsePrint(lTokens, i))
    case LEX_TOK_STRING:
        PARSE_TRY(parseString(lTokens, i))
    default:
        break;
    }
}

ParseErr parse(SVec *v, SVec *lTokens)
{
    for (uint32_t i = 0; i < svecLen(lTokens); i++)
    {
        ParseTok *tok = parseTok(lTokens, &i);
        if (tok)
        {
            svecAdd(v, tok);
            continue;
        }
        // FIXME: This will leak memory but...
        svecFree(lTokens);
        return PARSE_FAILED;
    }
    svecFree(lTokens);
    return PARSE_OK;
}