#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "pin.H"
#include <stdint.h>

typedef struct {
    uint8_t kind;       // argument type.
    uint8_t location;   // argument location (stack or register).
    uint16_t stack_ofs; // used for stack arguments.
    uint32_t mem_size;  // used for memory arguments.
} UDFType;

typedef struct {
    uint64_t addr;
    uint64_t nargs;
    UDFType ret_ty;
    UDFType arg_ty[0]; // 'nargs' elements.
} UDFFunc;

typedef struct {
    uint64_t nfuncs;
    UDFFunc funcs[0]; // 'nfuncs' elements.
} UDFSpec;

using namespace std;

INT32 Usage() {
    cerr << "(Usage)" << endl;
    return -1;
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
                break;
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
