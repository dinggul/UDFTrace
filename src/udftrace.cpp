#include "pin.H"
#include <iostream>
#include <fstream>
using namespace std;

INT32 Usage() {
    cerr << "nono" << endl;
    return -1;
}

VOID Trace(TRACE trace, VOID *v) {
    /*
    const BOOL print_args = KnobPrintArgs.Value();


    for (BBL bbl = TRACE_BblHead(trace); BBL_Valid(bbl); bbl = BBL_Next(bbl)) {
        INS tail = BBL_InsTail(bbl);

        if( INS_IsCall(tail) )
        {
            if( INS_IsDirectBranchOrCall(tail) )
            {
                const ADDRINT target = INS_DirectBranchOrCallTargetAddress(tail);
                cout << target << endl;
            }
            else
            {
                if( print_args )
                {
                    INS_InsertCall(tail, IPOINT_BEFORE, AFUNPTR(do_call_args_indirect),
                                   IARG_BRANCH_TARGET_ADDR, IARG_BRANCH_TAKEN,  IARG_G_ARG0_CALLER, IARG_END);
                }
                else
                {
                    INS_InsertCall(tail, IPOINT_BEFORE, AFUNPTR(do_call_indirect),
                                   IARG_BRANCH_TARGET_ADDR, IARG_BRANCH_TAKEN, IARG_END);
                }


            }
        }
        else
        {
            // sometimes code is not in an image
            RTN rtn = TRACE_Rtn(trace);

            // also track stup jumps into share libraries
            if( RTN_Valid(rtn) && !INS_IsDirectBranchOrCall(tail) && ".plt" == SEC_Name( RTN_Sec( rtn ) ))
            {
                const ADDRINT target = RTN_Address(rtn);
                cout << target << endl;
            }
        }

    }
    */
}

int main(int argc, char *argv[]) {
    if( PIN_Init(argc, argv) ) {
        return Usage();
    }

    cout << hex;
    cout.setf(ios::showbase);

    TRACE_AddInstrumentFunction(Trace, 0);
    PIN_StartProgram();

    return 0;
}
