#include "RamMapper.h"

using namespace std;

RamMapper::RamMapper(){

}

RamMapper::~RamMapper(){

}


void RamMapper::parseBenchmarkCircuits(char *logicalRAMsList, char * logicalBlockCount){

    //Parse logical block count file
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


    //Parse logical RAM file
    ifstream logicalRamFile;
    logicalRamFile.open(logicalRAMsList);
    if( logicalRamFile.is_open() ){
        //Pass through first 2 lines in file
        getline(logicalRamFile, input);
        cout << input << endl;
        getline(logicalRamFile, input);
        cout << input << endl;
    }
    
    int ramID;
    string ramMode;
    int depth, width;
    
    while( logicalRamFile >> circ_id >> ramID >> ramMode >> depth >> width ){
        circuit_array[circ_id].insertLogicalRAM(ramID, ramMode, depth, width);
    }

    logicalRamFile.close();


}

void RamMapper::mapPhysicalRAM(int architecture){

    for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
        (*i).mapBRAMS(architecture);
    }

}

void RamMapper::printAllCircuits(){

    // Print vector elements using iterators
    for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
        (*i).printCircuit();
    }

}
