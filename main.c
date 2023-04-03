#include <stdio.h>
#include <stdlib.h>

#include "simple.h"

void prt(char *str)
{
    printf("%s", str);
}

int main()
{
    SimpleVM vm = {
        .print = prt,
        .lines = {NULL},
        .pc = 0,
        .variables = {{.name = NULL}},
        .errorMsg = NULL,
    };

    printf("\n");

    while (1)
    {
        rep(&vm, "");

        char line[1024];

        fgets(line, 1024, stdin);

        rep(&vm, line);
    }

    printf("\n");
    return 0;
}