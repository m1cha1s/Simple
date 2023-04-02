#include <stdio.h>

#include "simple.h"

void prt(char *str)
{
    printf("%s", str);
}

int main()
{
    SimpleVM vm = {
        .print = prt,
    };

    rep(&vm, "");
    rep(&vm, "PRINT \"Hello, world!\"");
    printf("\n");
    return 0;
}