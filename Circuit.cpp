#include "Circuit.h"

using namespace std;

Circuit::Circuit(){

}

Circuit::~Circuit(){

}

void Circuit::setCircuitID(int id){
    circuit_id = id;
}

int Circuit::getCircuitID(){
    return circuit_id;
}

void Circuit::setLogicBlocks(int amount){
    num_logic_blocks = amount;
}

void Circuit::insertLogicalRAM(int id, string mode, int depth, int width){
    RAM ram;
    ram.setLogicalRAM(id, mode, depth, width);
    ram_array.push_back(ram);
}

void Circuit::mapBRAMS(int arch){

    for (auto i = ram_array.begin(); i != ram_array.end(); ++i){
        (*i).mapBRAMS(arch);
    }

}

long long Circuit::calcTotalArea(){


    
}

void Circuit::printCircuitMapping(ofstream& mapping_file){

    for (auto i = ram_array.begin(); i != ram_array.end(); ++i){
        mapping_file << getCircuitID() << " ";
        (*i).printRamMapping(mapping_file);
    }

}

void Circuit::printCircuit(){

    cout << circuit_id << "   " << num_logic_blocks << endl;

    for (auto i = ram_array.begin(); i != ram_array.end(); ++i){
        (*i).printLogicalRAMs();
    }
}
