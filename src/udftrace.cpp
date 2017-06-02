#include "pin.H"
#include <iostream>
#include <fstream>

INT32 Usage() {
    cerr << "nono" << endl;
    return -1;
}

int main(int argc, char *argv[]) {
    if( PIN_Init(argc, argv) ) {
        return Usage();
    }

    PIN_StartProgram();

    return 0;
}
