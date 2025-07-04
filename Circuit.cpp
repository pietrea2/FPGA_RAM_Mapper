#include "Circuit.h"

using namespace std;

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

void Circuit::sort_RAMS(){
    sort(ram_array.begin(), ram_array.end());
}

void Circuit::mapBRAMS(int arch, int size, int width, int ratio){

    //ram map: choose by smallest individual ram area
    for (auto i = ram_array.begin(); i != ram_array.end(); ++i){
        
        block_count = (*i).mapBRAMS(arch, size, width, ratio);
        
        LUT_blocks_used += block_count[0];
        BRAM_8K_used += block_count[1];
        BRAM_128K_used += block_count[2];
        BRAM_used += block_count[3];
        additional_LUTs += block_count[4];
        BRAM_1_used += block_count[5];
        BRAM_2_used += block_count[6];
    }

    calcTotalArea(arch, size, width, ratio);
}

void Circuit::clearMapping(){

    block_count.clear();
    LUT_blocks_used = 0;
    BRAM_8K_used = 0;
    BRAM_128K_used = 0;
    BRAM_used = 0;
    BRAM_1_used = 0;
    BRAM_2_used = 0;
    additional_LUTs = 0;
    total_FPGA_area = 0.0;

    for (auto i = ram_array.begin(); i != ram_array.end(); ++i){
        (*i).clearMapping();
    }
}

void Circuit::calcTotalArea(int arch, int size, int width, int ratio){

    long double area;

    if(arch == 1){

        int LUTRAM_ratio = 1;
        int BRAM_8K_ratio = 10;
        int BRAM_128K_ratio = 300;
        
        long long extra_logic_blocks = additional_LUTs / 10;
        if(additional_LUTs % 10) extra_logic_blocks += 1;

        long long LUT_logic_blocks = LUT_blocks_used * LUTRAM_ratio * 2;
        long long BRAM_8K_logic_blocks = BRAM_8K_used * BRAM_8K_ratio;
        long long BRAM_128K_logic_blocks = BRAM_128K_used * BRAM_128K_ratio;
        long long total_logic_blocks_required = extra_logic_blocks + num_logic_blocks + LUT_logic_blocks;

        vector<long long> logic_blocks = {LUT_logic_blocks, BRAM_8K_logic_blocks, BRAM_128K_logic_blocks, total_logic_blocks_required};
        long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

        int num_8K_BRAMs = (int)limiting_factor / BRAM_8K_ratio;
        int num_128K_BRAMs = (int)limiting_factor / BRAM_128K_ratio;

        long long area_LBs = limiting_factor * ( (LUT_logic_blocks != 0) ? 37500 : 35000 );
        long long area_8K = num_8K_BRAMs * calcRamArea(8192, 32);
        long long area_128K = num_128K_BRAMs * calcRamArea(131072, 128);

        area = area_LBs + area_8K + area_128K;
        total_FPGA_area = area;

    }
    else if(arch == 2){

        long long extra_logic_blocks = additional_LUTs / 10;
        if(additional_LUTs % 10) extra_logic_blocks += 1;
        
        long long BRAM_logic_blocks = BRAM_used * ratio;
        long long total_logic_blocks_required = extra_logic_blocks + num_logic_blocks;

        vector<long long> logic_blocks = {BRAM_logic_blocks, total_logic_blocks_required};
        long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

        int num_BRAMs = limiting_factor / ratio;

        long long area_LBs = limiting_factor * 35000;
        long long area_BRAM = num_BRAMs * calcRamArea(size, width);

        area = area_LBs + area_BRAM;
        total_FPGA_area = area;

    }
    else if(arch == 3){

        int LUTRAM_ratio = 1;
        
        long long extra_logic_blocks = additional_LUTs / 10;
        if(additional_LUTs % 10) extra_logic_blocks += 1;
        
        long long LUT_logic_blocks = LUT_blocks_used * LUTRAM_ratio * 2;
        long long BRAM_logic_blocks = BRAM_used * ratio;
        long long total_logic_blocks_required = extra_logic_blocks + num_logic_blocks + LUT_logic_blocks;

        vector<long long> logic_blocks = {LUT_logic_blocks, BRAM_logic_blocks, total_logic_blocks_required};
        long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

        int num_BRAMs = limiting_factor / ratio;

        long long area_LBs = limiting_factor * ( (LUT_logic_blocks != 0) ? 37500 : 35000 );
        long long area_BRAM = num_BRAMs * calcRamArea(size, width);

        area = area_LBs + area_BRAM;
        total_FPGA_area = area;

    }
    else if(arch == 4){
        
        int BRAM_1_ratio = 4;
        int BRAM_2_ratio = 64;
        
        long long extra_logic_blocks = additional_LUTs / 10;
        if(additional_LUTs % 10) extra_logic_blocks += 1;

        long long BRAM_1_logic_blocks = BRAM_1_used * BRAM_1_ratio;
        long long BRAM_2_logic_blocks = BRAM_2_used * BRAM_2_ratio;
        long long total_logic_blocks_required = extra_logic_blocks + num_logic_blocks;

        vector<long long> logic_blocks = {BRAM_1_logic_blocks, BRAM_2_logic_blocks, total_logic_blocks_required};
        long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

        int num_1_BRAMs = (int)limiting_factor / BRAM_1_ratio;
        int num_2_BRAMs = (int)limiting_factor / BRAM_2_ratio;

        long long area_LBs = limiting_factor * 35000;
        long long area_1 = num_1_BRAMs * calcRamArea(1024*4, 16);
        long long area_2 = num_2_BRAMs * calcRamArea(1024*64, 64);

        area = area_LBs + area_1 + area_2;
        total_FPGA_area = area;
    }

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
