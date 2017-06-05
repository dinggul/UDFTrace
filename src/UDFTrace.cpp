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

VOID showcall(ADDRINT addr)
{
    cout << "call " << addr << endl;
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
                        IARG_ADDRINT, addr,
                        IARG_END);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if( PIN_Init(argc, argv) ) {
        return Usage();
    }

    cout << hex;
    cout.setf(ios::showbase);

    int nfuncs = 1;
    UDFSpec* spec = (UDFSpec*) malloc(sizeof(UDFSpec) + nfuncs * sizeof(UDFFunc));
    spec->nfuncs = nfuncs;
    spec->funcs[0].addr = 0x400526; // test2 / fibo()
    spec->funcs[0].nargs = 1;

    TRACE_AddInstrumentFunction(Trace, spec);

    PIN_StartProgram();
    return 0;
}
