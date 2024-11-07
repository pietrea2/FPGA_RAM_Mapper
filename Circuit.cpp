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

    int LUTRAM_ratio = 1;
    int BRAM_8K_ratio = 10;
    int BRAM_128K_ratio = 300;

    long long LUT_logic_blocks = LUT_blocks_used * LUTRAM_ratio;
    long long BRAM_8K_logic_blocks = BRAM_8K_used * BRAM_8K_ratio;
    long long BRAM_128K_logic_blocks = BRAM_128K_used * BRAM_128K_ratio;
    long long total_logic_blocks_required = additional_LUTs + num_logic_blocks;

    vector<long long> logic_blocks = {LUT_logic_blocks, BRAM_8K_logic_blocks, BRAM_128K_logic_blocks, total_logic_blocks_required};
    long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

    int num_8K_BRAMs = limiting_factor / BRAM_8K_ratio;
    int num_128K_BRAMs = limiting_factor / BRAM_128K_ratio;

    long long area_LBs = limiting_factor * ( (LUT_logic_blocks != 0) ? 37500 : 35000 );
    long long are_8K = num_8K_BRAMs * calcRamArea(8192, 32);
    long long are_128K = num_128K_BRAMs * calcRamArea(131072, 128);

    total_FPGA_area = area_LBs + are_8K + are_128K;

    cout << "Circuit " << circuit_id << " area: " << scientific << total_FPGA_area << defaultfloat << endl;

    
}

long long Circuit::getCircuitArea(){
    return total_FPGA_area;
}

long long Circuit::calcRamArea(int size, int max_width){
    long long area = 9000 + 5*size + 90*sqrt(size) + 1200*max_width;
    return area;
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
