#include "RamMapper.h"

using namespace std;

RamMapper::RamMapper(){

}

RamMapper::~RamMapper(){

}


void RamMapper::parseBenchmarkCircuits(char *logicalRAMsList, char * logicalBlockCount){

    string input;
    ifstream logicalCountFile;
    logicalCountFile.open(logicalBlockCount);
    if( logicalCountFile.is_open() ){
        getLine(logicalCountFile, input);
        cout << input << endl;
    }
    
    int circ_id;
    int num_logic_blocks;
    while( logicalCountFile >> circ_id >> num_logic_blocks ){
        
    }

}

void RamMapper::mapPhysicalRAM(int architecture){

}
