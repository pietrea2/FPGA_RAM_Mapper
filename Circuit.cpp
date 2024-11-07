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

void Circuit::mapSingleBRAM(int size, int width, int ratio){

    for (auto i = ram_array.begin(); i != ram_array.end(); ++i){
        
        block_count = (*i).mapArch2BRAM(size, width, ratio);
        
        LUT_blocks_used += block_count[0];
        BRAM_8K_used += block_count[1];
        BRAM_128K_used += block_count[2];
        additional_LUTs += block_count[3];
    }

    calcTotalArea2(size, width, ratio);
}

void Circuit::clearMapping(){

    block_count.clear();
    LUT_blocks_used = 0;
    BRAM_8K_used = 0;
    BRAM_128K_used = 0;
    additional_LUTs = 0;

    regular_LBs_used = 0;
    required_LBs = 0;
    total_FPGA_area = 0.0;

    for (auto i = ram_array.begin(); i != ram_array.end(); ++i){
        (*i).clearMapping();
    }
}

void Circuit::calcTotalArea(){

    /*
    cout << "LUT_blocks_used: " << LUT_blocks_used
         << " 8Ks used: " << BRAM_8K_used
         << " 128ks used: " << BRAM_128K_used
         << " Additional LUTs needed: " << additional_LUTs;
    */

    int LUTRAM_ratio = 1;
    int BRAM_8K_ratio = 10;
    int BRAM_128K_ratio = 300;

    long long LUT_logic_blocks = LUT_blocks_used * LUTRAM_ratio;
    long long BRAM_8K_logic_blocks = BRAM_8K_used * BRAM_8K_ratio;
    long long BRAM_128K_logic_blocks = BRAM_128K_used * BRAM_128K_ratio;
    long long total_logic_blocks_required = additional_LUTs + num_logic_blocks + LUT_logic_blocks;

    vector<long long> logic_blocks = {LUT_logic_blocks, BRAM_8K_logic_blocks, BRAM_128K_logic_blocks, total_logic_blocks_required};
    long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

    int num_8K_BRAMs = limiting_factor / BRAM_8K_ratio;
    int num_128K_BRAMs = limiting_factor / BRAM_128K_ratio;

    long long area_LBs = limiting_factor * ( (LUT_logic_blocks != 0) ? 37500 : 35000 );
    long long are_8K = num_8K_BRAMs * calcRamArea(8192, 32);
    long long are_128K = num_128K_BRAMs * calcRamArea(131072, 128);
    
    long double area = area_LBs + are_8K + are_128K;
    total_FPGA_area = area;
    
    /*
    cout << " Tiles: " << limiting_factor << " ";
    cout << "Circuit " << circuit_id << " area: " << scientific;
    cout << area;
    cout << defaultfloat << endl;
    */
    
}

void Circuit::calcTotalArea2(int size, int width, int ratio){

    /*
    cout << "LUT_blocks_used: " << LUT_blocks_used
         << " 8Ks used: " << BRAM_8K_used
         << " 128ks used: " << BRAM_128K_used
         << " Additional LUTs needed: " << additional_LUTs;
    */
    long long BRAM_logic_blocks = LUT_blocks_used * ratio;
    long long total_logic_blocks_required = additional_LUTs + num_logic_blocks;

    vector<long long> logic_blocks = {BRAM_logic_blocks, total_logic_blocks_required};
    long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

    int num_BRAMs = limiting_factor / ratio;

    long long area_LBs = limiting_factor * 35000;
    long long area_BRAM = num_BRAMs * calcRamArea(size, width);
    
    long double area = area_LBs + area_BRAM;
    total_FPGA_area = area;
    
    /*
    cout << " Tiles: " << limiting_factor << " ";
    cout << "Circuit " << circuit_id << " area: " << scientific;
    cout << area;
    cout << defaultfloat << endl;
    */
    
}

long double Circuit::getCircuitArea(){
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
