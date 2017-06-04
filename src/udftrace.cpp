#include <fstream>
#include <iostream>

#include "pin.H"

using namespace std;

INT32 Usage() {
    cerr << "(Usage)" << endl;
    return -1;
}

VOID Routine(RTN rtn, VOID* v)
{
    cout << "Routine " << RTN_Address(rtn) << endl;
}

int main(int argc, char *argv[]) {
    if( PIN_Init(argc, argv) ) {
        return Usage();
    }

    cout << hex;
    cout.setf(ios::showbase);

    RTN_AddInstrumentFunction(Routine, NULL);

    PIN_StartProgram();
    return 0;
}
