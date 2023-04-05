#include "scompiler.h"
#include "sparser.h"
#include "svalue.h"

static CompilerErr compileTok(ParseTok *tok, VMProg *prog);

static CompilerErr compileString(ParseTok *tok, VMProg *prog)
{
    SValue *val = malloc(sizeof(SValue));

    val->type = SVALUE_STRING;
    val->val = tok->meta.value;

    svecAdd(&(prog->constPool), val);

    Op *op = malloc(sizeof(Op));

    op->type = OP_CONST_I;
    op->imm = svecLen(&(prog->constPool)) - 1;

    svecAdd(&(prog->prog), op);

    return COMPILER_OK;
}

static CompilerErr compilePrint(ParseTok *tok, VMProg *prog)
{
    if (compileTok(tok->meta.unary, prog))
        return COMPILER_ERR;

    Op *op = malloc(sizeof(Op));

    op->type = OP_PRINT;

    svecAdd(&(prog->prog), op);

    return COMPILER_OK;
}

typedef CompilerErr (*compileToken)(ParseTok *tok, VMProg *prog);

static const compileToken TOKENS[] = {
    compileString,
    compilePrint,
};

static CompilerErr compileTok(ParseTok *tok, VMProg *prog)
{
    if (tok->type >= (sizeof(TOKENS) / sizeof(compileToken)))
        return COMPILER_ERR;

    return TOKENS[tok->type](tok, prog);
}

CompilerErr compile(SVec *tokens, VMProg *prog)
{
    ParseTok *tok;

    while (tok = svecPop(tokens))
    {
        if (compileTok(tok, prog))
            return COMPILER_ERR;
    }
    return COMPILER_OK;
}