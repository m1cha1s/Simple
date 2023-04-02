#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simple.h"

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
            unsigned long j = i + 1;
            for (; j < strlen(src + i); j++)
                if (src[j] == ' ' || src[j] == '\0')
                    break;

            char *int_str = (char *)malloc(j - i + 1);
            strncpy(int_str, src + i, j - i);

            tok = (Token *)malloc(sizeof(TokenLitInt));
            tok->type = TOKEN_LIT_INT;
            ((TokenLitInt *)tok)->Int = atoi(int_str);

            free(int_str);

            return tok;
            break;
        }
        case 'G':
        case 'g':
        {
            if (!strncmp(src + i, "goto", 4) || !strncmp(src + i, "GOTO", 4))
            {
                tok = (Token *)malloc(sizeof(TokenGoto));
                tok->type = TOKEN_GOTO;
                ((TokenGoto *)tok)->value = scan(src + i + 4);
                return tok;
            }
            break;
        }
        case 'R':
        case 'r':
        {
            if (!strncmp(src + i, "run", 3) || !strncmp(src + i, "RUN", 3))
            {
                tok = (Token *)malloc(sizeof(Token));
                tok->type = TOKEN_RUN;
                return tok;
            }
            break;
        }
        case 'P':
        case 'p':
        {
            if (!strncmp(src + i, "print", 5) || !strncmp(src + i, "PRINT", 5))
            {
                tok = (Token *)malloc(sizeof(TokenPrint));
                tok->type = TOKEN_PRINT;
                ((TokenPrint *)tok)->value = scan(src + i + 5);
                return tok;
            }
            break;
        }
        case '"':
        {
            unsigned long j = i + 1;
            for (; j < strlen(src + i); j++)
                if (src[j] == '"')
                    break;

            tok = (Token *)malloc(sizeof(TokenLitString));
            tok->type = TOKEN_LIT_STRING;
            ((TokenLitString *)tok)->string = (char *)malloc(j - i);
            strncpy(((TokenLitString *)tok)->string, src + i + 1, j - i - 1);
            return tok;
            break;
        }
        default:
            break;
        }
    }
    return NULL;
}

int eval_prog(SimpleVM *vm)
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

int eval(SimpleVM *vm, char *src)
{
    Token *tok = scan(src);
    if (!tok)
        return 1;

    switch (tok->type)
    {
    case TOKEN_GOTO:
    {
        // FIXME: Check for correct type
        vm->pc = ((TokenLitInt *)(((TokenGoto *)tok)->value))->Int - 2; // -2 to accomodate for the line number offset and loop increment above
        return 0;
        break;
    }
    case TOKEN_RUN:
    {
        return eval_prog(vm);
        break;
    }
    case TOKEN_PRINT:
    {
        switch (((TokenPrint *)tok)->value->type)
        {
        case TOKEN_LIT_STRING:
        {
            vm->print("\n");
            vm->print(((TokenLitString *)((TokenPrint *)tok)->value)->string);
            return 0;
            break;
        }
        case TOKEN_LIT_INT:
        {
            int x = ((TokenLitInt *)((TokenPrint *)tok)->value)->Int;
            size_t lenght = snprintf(NULL, 0, "%d", x) + 1;
            char *str = malloc(lenght);
            snprintf(str, lenght, "%d", x);

            vm->print("\n");
            vm->print(str);

            free(str);
        }
        default:
            return 1;
            break;
        }
        break;
    }
    default:
        return 1;
        break;
    }
}

int rep(SimpleVM *vm, char *input)
{
    if (input[0] == '\0')
    {
        vm->print("\n>");
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
