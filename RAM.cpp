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

void RAM::mapBRAMS(int arch){

    if(logical_ram_mode != LogicalRamModes::TrueDualPort){
        mapLUTRAM();
    }
    mapBRAM8K(arch);
    mapBRAM128K(arch);


}

void RAM::mapLUTRAM(){

    //try both modes: 10bit wide and 20bit wide words
    long int best_area = 0;
    int LUT_depth_used;
    int LUT_width_used;

    long int cur_area;
    int S;
    int P;
    int num_LUT_blocks;
    int muxes;
    int decoders;
    int extra_LUTs;
    int extra_logic_blocks;
    
    int phys_depth = 64;
    int phys_width = 10;

    //loop for 64x 10-bit words and 32x 20-bit words
    for(int i = 0; i < 2; i++ ){

        //if need to increase depth of RAM, put blocks in Series
        if(logical_ram_depth > phys_depth){

            //Calc how many blocks in Series (S) and Parallel (P)
            S = logical_ram_depth / phys_depth;
            if(logical_ram_depth % phys_depth) S += 1;

            //only map if 16 or less blocks in series needed
            if(decoders <= 16){

                P = logical_ram_width / phys_width;
                if(logical_ram_width % phys_width) P += 1;

                //Calc extra logic needed 
                if(S > 2) decoders = S;
                else decoders = 1;
                muxes = logical_ram_width;

                extra_LUTs = decoders + muxes;
                extra_logic_blocks = extra_LUTs / 10;
            }
        }
        else{
            S = 1;
            muxes = 0;
            decoders = 0;
            extra_LUTs = 0;
            extra_logic_blocks = 0;

            P = logical_ram_width / phys_width;
            if(logical_ram_width % phys_width) P += 1;
        }

        //Calc area, set to best area (if smallest so far)
        cur_area = S * P * 40000 + extra_logic_blocks * 35000;
        if(best_area == 0 || cur_area < best_area ){
            best_area = cur_area;
            LUT_depth_used = phys_depth;
            LUT_width_used = phys_width;
        }

        phys_depth = 32;
        phys_width = 20;

    }

    additional_LUTs_needed = extra_LUTs;
    physical_ram_id = logical_ram_id;
    parallel_RAMs = P;
    series_RAMs = S;
    RAM_type = BRAMs::LUTRAM;
    physical_width = LUT_width_used;
    physical_depth = LUT_depth_used;
    ram_area = best_area;



    


    
    


}

void RAM::mapBRAM8K(int arch){

    //loop through all possible width and depth configuration

}

void RAM::mapBRAM128K(int arch){

    //loop through all possible width and depth configuration

}

long int RAM::calcRamArea(int LUTRAM_support, int bits, int max_width){

    long int logic_block_area;
    long int block_RAM_area;


}

void RAM::printLogicalRAMs(){

    cout << logical_ram_id << "   " << logical_ram_mode << "   " << logical_ram_depth << "    " << logical_ram_width << endl;
    
}
