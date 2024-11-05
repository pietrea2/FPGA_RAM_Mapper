#include "RAM.h"

using namespace std;

RAM::RAM(){

}

RAM::~RAM(){

}

void RAM::setLogicalRAM(int id, string mode, int depth, int width){

    logical_ram_id = id;
    logical_ram_depth = depth;
    logical_ram_width = width;

    if( mode == "ROM" ){
        logical_ram_mode = LogicalRamModes::ROM;
    }
    else if( mode == "SinglePort" ){
        logical_ram_mode = LogicalRamModes::SinglePort;
    }
    else if( mode == "SimpleDualPort" ){
        logical_ram_mode = LogicalRamModes::SimpleDualPort;
    }
    else if( mode == "TrueDualPort" ){
        logical_ram_mode = LogicalRamModes::TrueDualPort;
    }
    else{
        cout << "Invalid RAM Type!" << endl;
    }

}

string RAM::romToString(LogicalRamModes mode){

    string rom_mode;

    switch(mode){
        case LogicalRamModes::ROM:
            rom_mode = "ROM";
            break;
        case LogicalRamModes::SimpleDualPort:
            rom_mode = "SimpleDualPort";
            break;
        case LogicalRamModes::SinglePort:
            rom_mode = "SinglePort";
            break;
        case LogicalRamModes::TrueDualPort:
            rom_mode = "TrueDualPort";
            break;
    }

    return rom_mode;
}

void RAM::mapBRAMS(int arch){

    if(logical_ram_mode != LogicalRamModes::TrueDualPort){
        mapLUTRAM();
    }

    //mapBRAM8K(arch);
    //mapBRAM128K(arch);


}

int RAM::calcPhysicalBlocks(int logical_length, int physical_length){

    int num_blocks_needed = logical_length / physical_length;
    if(logical_length % physical_length) num_blocks_needed += 1;

    return num_blocks_needed;

}

void RAM::saveRamMapping(int additional_LUTs, int phys_ram_id, int p, int s, BRAMs ram_type, int phys_width, int phys_depth, int area){

    additional_LUTs_needed = additional_LUTs;
    physical_ram_id = phys_ram_id;
    parallel_RAMs = p;
    series_RAMs = s;
    BRAM_type = ram_type;
    physical_width = phys_width;
    physical_depth = phys_depth;
    ram_area = area;

}

void RAM::mapLUTRAM(){

    //try both modes: 10bit wide and 20bit wide words
    int LUT_depth_used;
    int LUT_width_used;

    long int cur_area;
    int S = 1;
    int P;
    int muxes = 0;
    int decoders = 0;
    int extra_LUTs = 0;
    int extra_logic_blocks = 0;
    
    int phys_depth = 64;
    int phys_width = 10;

    //loop for 64x 10-bit words and 32x 20-bit words
    for(int i = 0; i < 2; i++ ){

        //if need to increase depth of RAM, put blocks in Series
        if(logical_ram_depth > phys_depth){

            //Calc how many blocks in Series (S)
            S = calcPhysicalBlocks(logical_ram_depth, phys_depth);

            //only map if 16 or less blocks in series needed
            if(decoders <= 16){

                //Calc extra logic needed due to Series blocks
                if(S > 2) decoders = S;
                else decoders = 1;
                muxes = logical_ram_width;

                extra_LUTs = decoders + muxes;
                extra_logic_blocks = extra_LUTs / 10;
            }
            else continue;
        }
        
        //Calc how many blocks in Parallel (P)
        P = calcPhysicalBlocks(logical_ram_width, phys_width);
        

        //Calc area, set to best area (if smallest so far)
        cur_area = S * P * 40000 + extra_logic_blocks * 35000;
        if(ram_area == 0 || cur_area < ram_area ){
            saveRamMapping(extra_LUTs, logical_ram_id, P, S, BRAMs::LUTRAM, phys_width, phys_depth, cur_area);
        }

        //loop again trying these dimensions
        phys_depth = 32;
        phys_width = 20;
    }

}

void RAM::mapBRAM8K(int arch){

    /*
    //loop through all possible width and depth configuration
    int max_width;
    if(logical_ram_mode == LogicalRamModes::TrueDualPort) max_width = 16;
    else max_width = 32;

    for(){
        
    }
    */

}

void RAM::mapBRAM128K(int arch){

    //loop through all possible width and depth configuration

}

long int RAM::calcRamArea(int LUTRAM_support, int bits, int max_width){

    long int logic_block_area;
    long int block_RAM_area;


}

void RAM::printRamMapping(ofstream& mapping_file){

    mapping_file << logical_ram_id << " "
                 << additional_LUTs_needed << " "
                 << "LW " << logical_ram_width << " "
                 << "LD " << logical_ram_depth << " "
                 << "ID " << physical_ram_id << " "
                 << "S "  << series_RAMs << " "
                 << "P "  << parallel_RAMs << " "
                 << "Type " << BRAM_type << " "
                 << "Mode " << romToString(logical_ram_mode) << " "
                 << "W "  << physical_width << " "
                 << "D "  << physical_depth << endl;
}

void RAM::printLogicalRAMs(){

    cout << logical_ram_id << "   " << logical_ram_mode << "   " << logical_ram_depth << "    " << logical_ram_width << endl;
    
}
