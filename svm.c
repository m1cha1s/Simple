#include "svm.h"
#include "svalue.h"

static VMErr execConstI(Op *op, VM *vm, VMConfig *cfg, VMProg *prog)
{
    SValue *val;

    if (!(val = svecGet(&(prog->constPool), op->imm)))
        return VM_ERR;

    svecAdd(&(vm->stack), val);

    return VM_OK;
}

static VMErr execPrint(Op *op, VM *vm, VMConfig *cfg, VMProg *prog)
{
    SValue *val;
    if (!(val = svecPop(&(vm->stack))))
        return VM_ERR;

    if (SVALUE_IS_STRING(val))
    {
        cfg->print(SVALUE_AS_STRING(val));
        cfg->print("\n");
    }
    if (SVALUE_IS_INT(val))
    {
        cfg->print("INT");
        cfg->print("\n");
    }

    return VM_OK;
}

typedef VMErr (*execOp)(Op *op, VM *vm, VMConfig *cfg, VMProg *prog);

static const execOp EXEC_OP_TABLE[] = {
    execConstI,
    execPrint,
};

VMErr vmExec(VM *vm, VMConfig *cfg, VMProg *prog)
{
    Op *op;
    while (op = svecGet(&(prog->constPool), vm->pc))
    {
        if (op->type >= (sizeof(EXEC_OP_TABLE) / sizeof(execOp)))
            return VM_ERR;

        EXEC_OP_TABLE[vm->pc](op, vm, cfg, prog);

        vm->pc++;
    }
    return VM_OK;
}

void vmProgInit(VMProg *prog)
{
    svecInit(&(prog->constPool));
    svecInit(&(prog->prog));
}

void vmInit(VM *vm)
{
    vm->pc = 0;
    svecInit(&(vm->stack));
}

void vmProgFree(VMProg *prog)
{
    svecFree(&(prog->constPool));
    svecFree(&(prog->prog));
}

void vmFree(VM *vm)
{
    svecFree(&(vm->stack));
}
