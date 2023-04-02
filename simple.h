#ifndef _SIMPLE_H
#define _SIMPLE_H

#define MAX_VARIABLE_COUNT 100
#define MAX_LINE_COUNT 9999

typedef struct _Val
{
    enum
    {
        STRING_VALUE,
        INT_VALUE,
    } type;
    union
    {
        char *string;
        int Int;
    } value;
} Val;

typedef struct _Variable
{
    char *name;
    Val val;
} Variable;

typedef struct _SimpleVM
{
    void (*print)(char *);

    Variable variables[MAX_VARIABLE_COUNT];

    char *lines[MAX_LINE_COUNT];
    unsigned int last_line;
    unsigned int pc;

    char *errorMsg;
} SimpleVM;

typedef enum _TokenType
{
    TOKEN_GOTO,
    TOKEN_RUN,
    TOKEN_PRINT,
    TOKEN_EQUAL,
    TOKEN_VAR,
    TOKEN_VAL,

    TOKEN_LIT_STRING,
    TOKEN_LIT_INT,
} TokenType;

typedef struct _Token
{
    TokenType type;
} Token;

typedef struct _TokenVal
{
    TokenType type;
    Val value;
} TokenVal;

typedef struct _TokenPrint
{
    TokenType type;
    Token *value;
} TokenPrint;

typedef struct _TokenGoto
{
    TokenType type;
    Token *value;
} TokenGoto;

typedef struct _TokenVar
{
    TokenType type;
    char *name;
} TokenVar;

typedef struct _TokenEqual
{
    TokenType type;
    TokenVar *var;
    Token *value;
} TokenEqual;

typedef struct _TokenLitString
{
    TokenType type;
    char *string;
} TokenLitString;

typedef struct _TokenLitInt
{
    TokenType type;
    int Int;
} TokenLitInt;

int eval(SimpleVM *vm, char *cmd);
Token *scan(char *src);
int eval_prog(SimpleVM *vm);
int rep(SimpleVM *vm, char *input);

#endif
