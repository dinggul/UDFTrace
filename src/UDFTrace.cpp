#include <assert.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "pin.H"
#include "spec.h"

using namespace std;

KNOB<string> KnobInputFile(KNOB_MODE_WRITEONCE, "pintool",
        "i", "", "Trace spec file name");
static ostream& of = cout;

INT32 Usage() {
    cerr << "User Defined Function Trace (UDFTrace)" << endl;
    cerr << KNOB_BASE::StringKnobSummary() << endl;
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

static ADDRINT getArg(CONTEXT* ctx, UDFArg* ty)
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

VOID handleCall(CONTEXT* ctx, VOID* v)
{
    UDFFunc* func = (UDFFunc*)v;
    of << "sub_" << func->addr << " (";

    for (int i = 0; i < UDF_ARG_MAX; i ++)
    {
        if (func->args[i].type == UDF_VOID)
            break;
        if (i > 0)
            of << ", ";
        switch (func->args[i].type)
        {
            case UDF_CHAR:
                of << (char)(getArg(ctx, &func->args[i]) & 0xff);
                break;
            case UDF_I32:
                of << (getArg(ctx, &func->args[i]) & 0xffffffff);
                break;
            case UDF_U32:
                of << hex << (getArg(ctx, &func->args[i]) & 0xffffffff) << dec;
                break;
            case UDF_I64:
                of << getArg(ctx, &func->args[i]);
                break;
            case UDF_U64:
                of << hex << getArg(ctx, &func->args[i]) << dec;
                break;
            default:
                cerr << "Cannot handle arg type " << func->args[i].type << endl;
                return;
        }
    }
    of << ")" << endl;
}

static UDFFunc* lookupSpec(UDFSpec* spec, ADDRINT addr)
{
    for (unsigned int i = 0; i < spec->nfuncs; i ++) {
        if (spec->funcs[i].addr == addr) {
            return &spec->funcs[i];
        }
    }
    return NULL;
}

VOID Trace(TRACE trace, VOID* v)
{
    UDFSpec* spec = (UDFSpec*)v;

    for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl))
    {
        ADDRINT addr = BBL_Address(bbl);
        UDFFunc* func = lookupSpec(spec, addr);
        if (func) {
            BBL_InsertCall(bbl, IPOINT_BEFORE,
                    (AFUNPTR)handleCall,
                    IARG_CONST_CONTEXT,
                    IARG_PTR, func,
                    IARG_END);
        }
    }
}

static UDFSpec* loadSpec(const string& filename)
{
    UDFSpec* spec = NULL;

    if (filename.empty()) {
        int nfuncs = 1;
        spec = (UDFSpec*) malloc(sizeof(UDFSpec) + nfuncs * sizeof(UDFFunc));
        spec->nfuncs = nfuncs;
        spec->funcs[0].addr = 0x400526; // test2 / fibo()
        spec->funcs[0].ret.type = UDF_I64;
        spec->funcs[0].ret.loc = UDF_RAX;
        for (int i = 0; i < UDF_ARG_MAX; i ++)
            spec->funcs[0].args[i].type = UDF_VOID;
        spec->funcs[0].args[0].type = UDF_I64;
        spec->funcs[0].args[0].loc = UDF_RDI;
        return spec;
    }
    else {
        ifstream file(filename.c_str(), ios::binary | ios::ate);
        if (!file.is_open()) {
            cerr << "Cannot open file '" << filename << "'" << endl;
            return NULL;
        }

        streamsize size = file.tellg();
        file.seekg(0, ios::beg);

        spec = (UDFSpec*) malloc(size);
        file.read((char*)spec, size);
        return spec;
    }
}

int main(int argc, char *argv[])
{
    assert(sizeof(UDFArg) == 8 || "UDFArg is 8 bytes long");
    assert(sizeof(UDFFunc) == 64 || "UDFFunc is 64 bytes long");

    if(PIN_Init(argc, argv))
        return Usage();

    UDFSpec* spec = loadSpec(KnobInputFile.Value());

    if (spec == NULL)
        return Usage();

    of << showbase;

    TRACE_AddInstrumentFunction(Trace, spec);

    PIN_StartProgram();
    return 0;
}
