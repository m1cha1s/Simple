#include <stdio.h>
#include <stdlib.h>

// #include "simple.h"

// void prt(char *str)
// {
//     printf("%s", str);
// }

// int main()
// {
//     SimpleVM vm = {
//         .print = prt,
//         .lines = {NULL},
//         .pc = 0,
//         .variables = {{.name = NULL}},
//         .errorMsg = NULL,
//     };

//     printf("\n");

//     while (1)
//     {
//         rep(&vm, "");

//         char line[1024];

//         fgets(line, 1024, stdin);

//         rep(&vm, line);
//     }

//     printf("\n");
//     return 0;
// }

#include "svec.h"
#include "slexer.h"
#include "sparser.h"

int main()
{
    SVec lt, pt;
    svecInit(&lt);
    svecInit(&pt);

    LexErr le = lex(&lt, "print\"Hello\"");

    for (int i = 0; i < svecLen(&lt); i++)
    {
        LexTok *tok = svecGet(&lt, i);
        if (tok->type == LEX_TOK_PRINT)
            printf("%d PRINT_TOKEN\n", i);
        if (tok->type == LEX_TOK_STRING)
            printf("%d STRING_TOKEN: %s\n", i, (char *)(tok->meta));
    }

    ParseErr pe = parse(&pt, &lt);

    for (int i = 0; i < svecLen(&pt); i++)
    {
        ParseTok *tok = svecGet(&pt, i);
        if (tok->type == PARSE_TOK_PRINT)
            printf("%d PRINT_TOKEN: %s\n", i, (char *)(tok->meta.unary->meta.value));
    }
}