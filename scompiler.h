#ifndef _SCOMPILER_H
#define _SCOMPILER_H

#include "scommon.h"
#include "svm.h"
#include "svec.h"

typedef enum _CompilerErr
{
    COMPILER_OK,
    COMPILER_ERR,
} CompilerErr;

CompilerErr compile(SVec *tokens, VMProg *prog);

#endif