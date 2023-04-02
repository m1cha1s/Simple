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
    };

    while (1)
    {
        rep(&vm, "");

        char *line = NULL;
        size_t size = 0;
        getline(&line, &size, stdin);

        printf("%s\n", line);

        rep(&vm, line);

        free(line);
    }

    printf("\n");
    return 0;
}