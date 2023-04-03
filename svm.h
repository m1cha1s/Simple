#ifndef _SVM_H
#define _SVM_H

#include "scommon.h"
#include "svec.h"

typedef enum _OpType
{
    CONST_I,
    PRINT,
} OpType;

typedef struct _Op
{
    OpType type;
    uint32_t imm;
} Op;

typedef struct _VM
{
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

VMErr vmExec(VMConfig *cfg, VMProg *prog);

#endif