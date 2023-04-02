#ifndef _SIMPLE_H
#define _SIMPLE_H

#define MAX_VARIABLE_COUNT 100
#define MAX_LINE_COUNT 9999

typedef struct _Variable
{
    char *name;
    enum
    {
        Int,
    } type;
    union
    {
        int Int;
    } value;
} Variable;

typedef struct _SimpleVM
{
    void (*print)(char *);

    Variable variables[MAX_VARIABLE_COUNT];

    char *lines[MAX_LINE_COUNT];
    unsigned int last_line;
    unsigned int pc;
} SimpleVM;

typedef enum _TokenType
{
    TOKEN_RUN,
    TOKEN_PRINT,
    TOKEN_LIT_STRING,
} TokenType;

typedef struct _Token
{
    TokenType type;
} Token;

typedef struct _TokenPrint
{
    TokenType type;
    Token *value;
} TokenPrint;

typedef struct _TokenLitString
{
    TokenType type;
    char *string;
} TokenLitString;

int eval(SimpleVM *vm, char *cmd);
Token *scan(char *src);
int eval_prog(SimpleVM *vm);
int rep(SimpleVM *vm, char *input);

#endif
