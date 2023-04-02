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

// int eval_prog(SimpleVM *vm)
// {
// }

int eval(SimpleVM *vm, char *src)
{
    Token *tok = scan(src);

    switch (tok->type)
    {
    case TOKEN_PRINT:
    {
        switch (((TokenPrint *)tok)->value->type)
        {
        case TOKEN_LIT_STRING:
        {
            vm->pc = 0;
            vm->print("\n");
            vm->print(((TokenLitString *)((TokenPrint *)tok)->value)->string);
            return 0;
            break;
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

        int line_num = atoi(line_num_str);

        free(line_num_str);

        vm->lines[line_num] = malloc(strlen(input) - i);
        strcpy(vm->lines[line_num], input + i);

        return 0;
    }
    return eval(vm, input);
}
