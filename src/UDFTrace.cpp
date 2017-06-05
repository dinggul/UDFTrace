#include <assert.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "pin.H"
#include "spec.h"

using namespace std;

INT32 Usage() {
    cerr << "(Usage)" << endl;
    return -1;
}

static REG convertRegId(int udf_reg)
{
    switch (udf_reg) {
        case UDF_RAX: return REG_RAX;
        case UDF_RBX: return REG_RBX;
        case UDF_RCX: return REG_RCX;
        case UDF_RDX: return REG_RDX;
        case UDF_RDI: return REG_RDI;
        case UDF_RSI: return REG_RSI;
        case UDF_RBP: return REG_RBP;
        case UDF_RSP: return REG_RSP;
        case UDF_R8: return REG_R8;
        case UDF_R9: return REG_R9;
        case UDF_R10: return REG_R10;
        case UDF_R11: return REG_R11;
        case UDF_R12: return REG_R12;
        case UDF_R13: return REG_R13;
        case UDF_R14: return REG_R14;
        case UDF_R15: return REG_R15;
    }
    return REG_RAX;
}

static ADDRINT getArg(CONTEXT* ctx, UDFType* ty)
{
    if (ty->loc == UDF_STACK) {
        cerr << "Cannot handle stack argument" << endl;
        return 0;
    }
    else {
        REG reg = convertRegId(ty->loc);
        ADDRINT val;
        PIN_GetContextRegval(ctx, reg, (UINT8*)&val);
        return val;
    }
}

VOID showcall(CONTEXT* ctx, VOID* v)
{
    UDFFunc* func = (UDFFunc*)v;
    cout << "call " << func->addr << endl;

    for (int i = 0; i < UDF_ARG_MAX; i ++)
    {
        if (func->args[i].kind == UDF_VOID)
            break;
        switch (func->args[i].kind)
        {
            case UDF_I64:
            case UDF_U64:
                cout << "  " << hex << getArg(ctx, &func->args[i]) << endl;
                break;
            default:
                cerr << "Cannot handle arg type " << func->args[i].kind << endl;
        }
    }
}

VOID Trace(TRACE trace, VOID* v)
{
    UDFSpec* spec = (UDFSpec*)v;

    for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
    {
        ADDRINT addr = BBL_Address(bbl);
        for (unsigned int i = 0; i < spec->nfuncs; i ++) {
            if (spec->funcs[i].addr == addr) {
                cout << "Instrumenting " << addr << endl;
                BBL_InsertCall(bbl, IPOINT_BEFORE, (AFUNPTR)showcall,
                        IARG_CONST_CONTEXT,
                        IARG_PTR, (VOID*)&spec->funcs[i],
                        IARG_END);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    assert(sizeof(UDFType) == 8 || "UDFType is 8 bytes long");
    assert(sizeof(UDFFunc) == 64 || "UDFFunc is 64 bytes long");

    if(PIN_Init(argc, argv))
        return Usage();

    int nfuncs = 1;
    UDFSpec* spec = (UDFSpec*) malloc(sizeof(UDFSpec) + nfuncs * sizeof(UDFFunc));
    spec->nfuncs = nfuncs;
    spec->funcs[0].addr = 0x400526; // test2 / fibo()
    spec->funcs[0].ret.kind = UDF_I64;
    spec->funcs[0].ret.loc = UDF_RAX;
    for (int i = 0; i < UDF_ARG_MAX; i ++)
        spec->funcs[0].args[i].kind = UDF_VOID;
    spec->funcs[0].args[0].kind = UDF_I64;
    spec->funcs[0].args[0].loc = UDF_RDI;

    TRACE_AddInstrumentFunction(Trace, spec);

    PIN_StartProgram();
    return 0;
}
