#ifndef _SVM_H
#define _SVM_H

#include "scommon.h"
#include "svec.h"

typedef enum _OpType
{
    OP_CONST_I = 0,
    OP_PRINT = 1,
} OpType;

typedef struct _Op
{
    OpType type;
    uint32_t imm;
} Op;

typedef struct _VM
{
    SVec stack;
    uint32_t pc;
} VM;

typedef struct _VMConfig
{
    void (*print)(char *);
} VMConfig;

typedef struct _VMProg
{
    SVec constPool;
    SVec prog;
} VMProg;

typedef enum _VMErr
{
    VM_OK,
    VM_ERR,
} VMErr;

void vmProgInit(VMProg *prog);
void vmInit(VM *vm);

void vmProgFree(VMProg *prog);
void vmFree(VM *vm);

VMErr vmExec(VM *vm, VMConfig *cfg, VMProg *prog);

#endif