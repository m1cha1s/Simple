#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "simple.h"

#define ERR(err, clean, ret)  \
    {                         \
        vm->errorMsg = (err); \
        (clean);              \
        return (ret);         \
    }

#define CHECK_ERR(ret)    \
    {                     \
        if (vm->errorMsg) \
            return (ret); \
    }

static void
cleanupToken(Token *token)
{
    if (!token)
        return;
    switch (token->type)
    {
    case TOKEN_PRINT:
    {
        TokenPrint *tok = (TokenPrint *)token;
        cleanupToken(tok->value);

        free(tok);
        break;
    }
    case TOKEN_GOTO:
    {
        TokenGoto *tok = (TokenGoto *)token;
        cleanupToken(tok->value);
        free(tok);
        break;
    }
    case TOKEN_EQUAL:
    {
        TokenEqual *tok = (TokenEqual *)token;

        cleanupToken(tok->value);
        cleanupToken((Token *)(tok->var));

        free(tok);
        break;
    }
    case TOKEN_VAR:
    {
        TokenVar *tok = (TokenVar *)token;

        free(tok->name);
        free(tok);
        break;
    }
    case TOKEN_LIT_INT:
    {
        TokenLitInt *tok = (TokenLitInt *)token;

        free(tok);
        break;
    }
    case TOKEN_LIT_STRING:
    {
        TokenLitString *tok = (TokenLitString *)token;

        free(tok->string);
        free(tok);
        break;
    }
    default:
    {
        free(token);
        break;
    }
    }
}

static void cleanupVariable(Variable *var, bool withName)
{
    if (!var)
        return;

    if (withName)
        free(var->name);

    switch (var->val.type)
    {
    case STRING_VALUE:
    {
        if (var->val.value.string)
            free(var->val.value.string);
        break;
    }
    default:
        break;
    }
}

Token *scan(char *src);

TokenLitInt *scanInt(char *src)
{
    TokenLitInt *tok;
    unsigned long j = 1;
    for (; j < strlen(src); j++)
        if (src[j] == ' ' || src[j] == '\0')
            break;

    char *int_str = (char *)malloc(j + 1);
    strncpy(int_str, src, j);
    int_str[j] = '\0';

    tok = malloc(sizeof(TokenLitInt));
    tok->type = TOKEN_LIT_INT;
    tok->Int = atoi(int_str);

    free(int_str);

    return tok;
}

TokenGoto *scanGoto(char *src)
{
    TokenGoto *tok = NULL;
    if (!strncmp(src, "goto", 4) || !strncmp(src, "GOTO", 4))
    {
        tok = malloc(sizeof(TokenGoto));
        tok->type = TOKEN_GOTO;
        tok->value = scan(src + 4);
    }
    return tok;
}

Token *scanRun(char *src)
{
    Token *tok = NULL;
    if (!strncmp(src, "run", 3) || !strncmp(src, "RUN", 3))
    {
        tok = (Token *)malloc(sizeof(Token));
        tok->type = TOKEN_RUN;
    }
    return tok;
}

TokenPrint *scanPrint(char *src)
{
    TokenPrint *tok = NULL;
    if (!strncmp(src, "print", 5) || !strncmp(src, "PRINT", 5))
    {
        tok = malloc(sizeof(TokenPrint));
        tok->type = TOKEN_PRINT;
        tok->value = scan(src + 5);
    }
    return tok;
}

TokenLitString *scanLitString(char *src)
{
    TokenLitString *tok = NULL;
    unsigned long j = 1;
    for (; j < strlen(src); j++)
        if (src[j] == '"')
            break;

    tok = malloc(sizeof(TokenLitString));
    tok->type = TOKEN_LIT_STRING;
    tok->string = (char *)malloc(j);
    strncpy(tok->string, src + 1, j - 1);
    tok->string[j - 1] = '\0';

    return tok;
}

TokenVar *scanVar(char *src)
{
    TokenVar *tok = NULL;
    unsigned long j = 1;
    for (; j < strlen(src); j++)
        if (src[j] == ' ' || src[j] == '\0' || src[j] == '\n' || src[j] == '=' || src[j] == '+' || src[j] == '-' || src[j] == '*' || src[j] == '/')
            break;

    tok = malloc(sizeof(TokenVar));
    tok->type = TOKEN_VAR;
    tok->name = (char *)malloc(j + 1);
    strncpy(tok->name, src, j);
    tok->name[j] = '\0';
    return tok;
}

TokenEqual *scanEqual(char *src)
{
    TokenEqual *tok = NULL;

    unsigned long j = 1;
    for (; j < strlen(src); j++)
        if (src[j] == ' ' || src[j] == '=')
        {
            j--;
            break;
        }
    if (src[j] == '\0')
        return tok;

    unsigned long k = j + 1;
    for (; k < strlen(src); k++)
        if (src[k] == '=')
            break;
    if (src[k] == '\0')
        return tok;

    tok = malloc(sizeof(TokenEqual));
    tok->type = TOKEN_EQUAL;
    tok->var = scanVar(src);
    tok->value = scan(src + k + 1);
    return tok;
}

TokenPlus *scanPlus(char *src)
{
    TokenPlus *tok = NULL;

    unsigned long j = 1;
    for (; j < strlen(src); j++)
        if (src[j] == ' ' || src[j] == '+')
        {
            j--;
            break;
        }
    if (src[j] == '\0')
        return tok;

    unsigned long k = j + 1;
    for (; k < strlen(src); k++)
        if (src[k] == '+')
            break;
    if (src[k] == '\0')
        return tok;

    tok = malloc(sizeof(TokenPlus));
    tok->type = TOKEN_PLUS;

    char *lVal = (char *)malloc(j + 2);
    lVal[j + 1] = '\0';
    strncpy(lVal, src, j + 1);
    tok->lVal = scan(lVal);
    free(lVal);
    tok->rVal = scan(src + k + 1);
    return tok;
}

Token *scanList(char *src)
{
    Token *tok = NULL;
    if (!strncmp(src, "list", 4) || !strncmp(src, "LIST", 4))
    {
        tok = (Token *)malloc(sizeof(Token));
        tok->type = TOKEN_LIST;
    }
    return tok;
}

Token *scan(char *src)
{
    Token *tok;
    for (unsigned long i = 0; i < strlen(src); i++)
    {
        switch (src[i])
        {
        case '\0':
            return NULL;
        case ' ':
        {
            continue;
            break;
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            return (Token *)scanInt(src + i);
            break;
        }
        case 'L':
        case 'l':
        {
            tok = scanList(src + i);
            if (tok)
                return tok;
            tok = (Token *)scanEqual(src + i);
            if (tok)
                return tok;
            tok = (Token *)scanPlus(src + i);
            if (tok)
                return tok;
            break;
        }
        case 'G':
        case 'g':
        {
            tok = (Token *)scanEqual(src + i);
            if (tok)
                return tok;
            tok = (Token *)scanPlus(src + i);
            if (tok)
                return tok;
            tok = (Token *)scanGoto(src + i);
            if (tok)
                return tok;
            return (Token *)scanVar(src + i);
            break;
        }
        case 'R':
        case 'r':
        {
            tok = (Token *)scanEqual(src + i);
            if (tok)
                return tok;
            tok = (Token *)scanPlus(src + i);
            if (tok)
                return tok;
            tok = scanRun(src + i);
            if (tok)
                return tok;
            return (Token *)scanVar(src + i);
            break;
        }
        case 'P':
        case 'p':
        {
            tok = (Token *)scanEqual(src + i);
            if (tok)
                return tok;
            tok = (Token *)scanPlus(src + i);
            if (tok)
                return tok;
            tok = (Token *)scanPrint(src + i);
            if (tok)
                return tok;
            return (Token *)scanVar(src + i);
            break;
        }
        case '"':
        {
            return (Token *)scanLitString(src + i);
            break;
        }
        default:
        {
            tok = (Token *)scanEqual(src + i);
            if (tok)
                return tok;
            tok = (Token *)scanPlus(src + i);
            if (tok)
                return tok;
            return (Token *)scanVar(src + i);
            break;
        }
        }
    }
    return NULL;
}

int evalProg(SimpleVM *vm)
{
    for (vm->pc = 0; vm->pc <= vm->last_line; vm->pc++)
    {
        if (!vm->lines[vm->pc])
            continue;

        if (eval(vm, vm->lines[vm->pc]))
            return 1;
    }
    return 0;
}

Variable *newVariable(SimpleVM *vm, char *name)
{
    unsigned int i = 0;
    for (; i < MAX_VARIABLE_COUNT; i++)
    {

        if (!(vm->variables[i].name))
        {
            vm->variables[i].name = malloc(strlen(name) + 1);
            strcpy(vm->variables[i].name, name);
            return &(vm->variables[i]);
            break;
        }
    }
    return NULL;
}

Variable *findVariable(SimpleVM *vm, char *name)
{
    unsigned int i = 0;
    for (; i < MAX_VARIABLE_COUNT; i++)
    {

        if (!(vm->variables[i].name))
            break;

        if (!strcmp(vm->variables[i].name, name))
        {
            return &(vm->variables[i]);
            break;
        }
    }

    return NULL;
}

Token *evalToken(SimpleVM *vm, Token *tok)
{
    if (!tok)
        return NULL;
    switch (tok->type)
    {
    case TOKEN_LIST:
    {
        for (int i = 0; i < MAX_LINE_COUNT; i++)
        {
            if (!(vm->lines[i]))
                continue;

            size_t lenght = snprintf(NULL, 0, "\t%4d %s", i + 1, vm->lines[i]) + 1;
            char *str = malloc(lenght);
            if (!str)
            {
                vm->errorMsg = "[evalToken TOKEN_LIST] Allocation failed!";
                return NULL;
            }
            snprintf(str, lenght, "\t%4d %s", i + 1, vm->lines[i]);

            vm->print(str);

            free(str);
        }
        return NULL;
        break;
    }
    case TOKEN_VAR:
    {
        TokenVar *var = (TokenVar *)tok;

        Variable *v = findVariable(vm, var->name);
        if (!v)
        {
            vm->errorMsg = "[evalToken TOKEN_VAR] Variable not found!";
            return NULL;
        }

        TokenVal *val = malloc(sizeof(TokenVal));

        val->type = TOKEN_VAL;
        val->value = v->val;

        cleanupToken((Token *)var);
        return (Token *)val;

        break;
    }
    case TOKEN_PLUS:
    {
        TokenPlus *add = (TokenPlus *)tok;

        TokenVal *lVal = (TokenVal *)evalToken(vm, add->lVal);
        CHECK_ERR(NULL)
        TokenVal *rVal = (TokenVal *)evalToken(vm, add->rVal);
        CHECK_ERR(NULL)

        add->lVal = (Token *)lVal;
        add->rVal = (Token *)rVal;

        if (!rVal || !lVal)
            ERR("[evalToken TOKEN_PLUS] Invalid token!", cleanupToken((Token *)add), NULL)

        if (rVal->value.type != lVal->value.type)
        {
            ERR("[evalToken TOKEN_PLUS] Type missmatch!", cleanupToken((Token *)add), NULL)
        }

        TokenVal *res = malloc(sizeof(TokenVal));
        res->type = TOKEN_VAL;

        if (rVal->value.type == INT_VALUE)
        {
            res->value.type = INT_VALUE;
            res->value.value.Int = lVal->value.value.Int + rVal->value.value.Int;
        }

        cleanupToken((Token *)add);
        return (Token *)res;
        break;
    }
    case TOKEN_EQUAL:
    {
        TokenVar *var = (TokenVar *)(((TokenEqual *)tok)->var);
        Token *value = (Token *)(((TokenEqual *)tok)->value);

        Variable *v = findVariable(vm, var->name);

        if (!v)
            v = newVariable(vm, var->name);

        value = evalToken(vm, value);
        if (!value)
        {
            vm->errorMsg = "[evalToken TOKEN_EQUAL] Invalid token!";
            return NULL;
        }

        cleanupVariable(v, false);

        switch (value->type)
        {
        case TOKEN_VAL:
        {
            TokenVal *tv = ((TokenVal *)value);

            switch (tv->value.type)
            {
            case STRING_VALUE:
            {
                v->val.type = STRING_VALUE;
                v->val.value.string = malloc(strlen(tv->value.value.string) + 1);
                strcpy(v->val.value.string, tv->value.value.string);

                break;
            }
            case INT_VALUE:
            {
                v->val.type = INT_VALUE;
                v->val.value.Int = tv->value.value.Int;

                break;
            }
            default:
            {
                vm->errorMsg = "[evalToken TOKEN_EQUAL] Unkown value type!";
                return NULL;
            }
            }

            cleanupToken(value);
            break;
        }
        default:
            vm->errorMsg = "[evalToken TOKEN_EQUAL] Unexpected token type!";
            cleanupToken(value);
            break;
        }

        // This prevents double free
        ((TokenEqual *)tok)->value = NULL;

        cleanupToken(tok);
        return NULL;
        break;
    }
    case TOKEN_GOTO:
    {
        TokenVal *val = (TokenVal *)evalToken(vm, ((TokenGoto *)tok)->value);
        ((TokenGoto *)tok)->value = (Token *)val;
        if (!val)
            goto TOKEN_GOTO_ERR;
        if (val->type != TOKEN_VAL)
            goto TOKEN_GOTO_ERR;
        if (val->value.type != INT_VALUE)
        {
            vm->errorMsg = "[evalToken TOKEN_GOTO] Expected int value!";
            return NULL;
            break;
        }

        vm->pc = val->value.value.Int - 2; // -2 to accomodate for the line number offset and loop increment above
        cleanupToken(tok);
        return NULL;
        break;
    TOKEN_GOTO_ERR:
        vm->errorMsg = "[evalToken TOKEN_GOTO] Expected value/variable!";
        return NULL;
        break;
    }
    case TOKEN_RUN:
    {
        evalProg(vm);
        cleanupToken(tok);
        return NULL;
        break;
    }
    case TOKEN_PRINT:
    {
        TokenVal *val = (TokenVal *)evalToken(vm, ((TokenPrint *)tok)->value);
        CHECK_ERR(NULL);
        if (!val)
        {
            vm->errorMsg = "[evalToken TOKEN_PRINT] Invalid token!";
            return NULL;
        }

        // This makes cleanup possible
        ((TokenPrint *)tok)->value = (Token *)val;

        Val v = val->value;

        switch (v.type)
        {
        case STRING_VALUE:
        {
            vm->print(v.value.string);
            vm->print("\n");
            break;
        }
        case INT_VALUE:
        {
            int x = v.value.Int;
            size_t lenght = snprintf(NULL, 0, "%d", x) + 1;
            char *str = malloc(lenght);
            if (!str)
            {
                vm->errorMsg = "[evalToken TOKEN_PRINT] Allocation failed!";
                return NULL;
            }
            snprintf(str, lenght, "%d", x);

            vm->print(str);
            vm->print("\n");

            free(str);
            break;
        }
        default:
        {
            vm->errorMsg = "[evalToken TOKEN_PRINT] Unkown value type!";
            return NULL;
        }
        }
        cleanupToken(tok);
        return NULL;
        break;
    }
    case TOKEN_LIT_STRING:
    {
        TokenVal *val = malloc(sizeof(TokenVal));
        val->type = TOKEN_VAL;

        val->value.type = STRING_VALUE;
        val->value.value.string = malloc(strlen(((TokenLitString *)tok)->string) + 1);
        // val->value.value.string[strlen(((TokenLitString *)tok)->string)] = '\0';
        strcpy(val->value.value.string, ((TokenLitString *)tok)->string);

        cleanupToken(tok);

        return (Token *)val;
        break;
    }
    case TOKEN_LIT_INT:
    {
        TokenVal *val = malloc(sizeof(TokenVal));
        val->type = TOKEN_VAL;

        val->value.type = INT_VALUE;
        val->value.value.Int = ((TokenLitInt *)tok)->Int;

        cleanupToken(tok);

        return (Token *)val;
        break;
    }
    default:
        cleanupToken(tok);
        vm->errorMsg = "[evalToken] Unknown token!";
        return NULL;
        break;
    }
}

int eval(SimpleVM *vm, char *src)
{
    Token *tok = scan(src);
    if (!tok)
    {
        vm->errorMsg = "[EVAL ERR] Unknown token!";
    }
    if (!(vm->errorMsg))
        evalToken(vm, tok);

    if (vm->errorMsg)
    {
        size_t lenght = snprintf(NULL, 0, "Line: %d %s\n", vm->pc, vm->errorMsg) + 1;
        char *str = malloc(lenght);
        if (!str)
        {
            vm->errorMsg = "[EVAL ERR] Allocation failed!";
            return 1;
        }
        snprintf(str, lenght, "Line: %d %s\n", vm->pc, vm->errorMsg);

        vm->print(str);

        free(str);
        return 1;
    }

    return 0;
}

int rep(SimpleVM *vm, char *input)
{
    if (input[0] == '\0')
    {
        vm->print(">");
        return 0;
    }
    if ((input[0] >= '0') && (input[0] <= '9'))
    {
        unsigned long i = 0;
        for (; i < strlen(input); i++)
            if (input[i] == ' ')
                break;

        // TODO: Handle malloc failure
        char *line_num_str = malloc(i + 1);
        strncpy(line_num_str, input, i);
        line_num_str[i] = '\0';

        unsigned int line_num = atoi(line_num_str) - 1;

        free(line_num_str);

        vm->lines[line_num] = malloc(strlen(input) - i);
        strcpy(vm->lines[line_num], input + i + 1);

        if (vm->last_line < line_num)
            vm->last_line = line_num;

        return 0;
    }
    return eval(vm, input);
}
