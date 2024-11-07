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
        
        block_count = (*i).mapBRAMS(arch);
        
        LUT_blocks_used += block_count[0];
        BRAM_8K_used += block_count[1];
        BRAM_128K_used += block_count[2];
        additional_LUTs += block_count[3];
    }

    calcTotalArea();

}

void Circuit::calcTotalArea(){

    cout << "LUT_blocks_used: " << LUT_blocks_used
         << " 8Ks used: " << BRAM_8K_used
         << " 128ks used: " << BRAM_128K_used
         << " Additional LUTs needed: " << additional_LUTs << endl;





    
}

long long Circuit::getCircuitArea(){
    return total_FPGA_area;
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
