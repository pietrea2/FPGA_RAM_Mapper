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
        getline(logicalCountFile, input);
        cout << input << endl;
    }
    
    int circ_id;
    int num_logic_blocks;
    while( logicalCountFile >> circ_id >> num_logic_blocks ){
        Circuit circuit;
        circuit.setCircuitID(circ_id);
        circuit.setLogicBlocks(num_logic_blocks);
        circuit_array.push_back(circuit);

    }

    logicalCountFile.close();

}

void RamMapper::mapPhysicalRAM(int architecture){

}

void RamMapper::printAllCircuits(){

    // Print vector elements using iterators
    for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
        (*i).printCircuit();
    }

}
