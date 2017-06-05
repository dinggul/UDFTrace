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

VOID showcall(CONTEXT* ctx)
{
    ADDRINT addr = (ADDRINT)PIN_GetContextReg(ctx, REG_INST_PTR);
    cout << "call " << addr << endl;
    for (int reg = REG_GR_BASE; reg <= REG_GR_LAST; reg ++)
    {
        ADDRINT val;
        PIN_GetContextRegval(ctx, (REG)reg, reinterpret_cast<UINT8*>(&val));
        cout << REG_StringShort((REG)reg) << ": " << val << endl;
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
                        IARG_CONST_CONTEXT, IARG_END);
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

    cout << hex;
    cout.setf(ios::showbase);

    int nfuncs = 1;
    UDFSpec* spec = (UDFSpec*) malloc(sizeof(UDFSpec) + nfuncs * sizeof(UDFFunc));
    spec->nfuncs = nfuncs;
    spec->funcs[0].addr = 0x400526; // test2 / fibo()
    spec->funcs[0].ret.kind = UDF_I64;
    spec->funcs[0].ret.loc = UDF_RAX;
    spec->funcs[0].args[0].kind = UDF_I64;
    spec->funcs[0].args[0].loc = UDF_RDI;

    TRACE_AddInstrumentFunction(Trace, spec);

    PIN_StartProgram();
    return 0;
}
