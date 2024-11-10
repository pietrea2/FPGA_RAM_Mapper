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

vector<long long> RAM::mapBRAMS2(int arch, int size, int width, int ratio){

    switch (arch) {
        case 1:
            if(logical_ram_mode != LogicalRamModes::TrueDualPort) mapBRAM2(LUTRAM, 640, 20, 1);
            mapBRAM2(BRAM8K, 8192, 32, 10);
            mapBRAM2(BRAM128K, 131072, 128, 300);
            break;
        case 2:
            mapBRAM2(BRAM_NOLUT, size, width, ratio);
            break;
        case 3:
            if(logical_ram_mode != LogicalRamModes::TrueDualPort) mapBRAM2(LUTRAM, 640, 20, 1);
            mapBRAM2(BRAM_WITHLUT, size, width, ratio);
            break;
        default:
            break;
    }

    long long LUTRAM_amount = 0;
    long long BRAM8K_amount = 0;
    long long BRAM128K_amount = 0;
    long long BRAM_amount = 0;

    switch (BRAM_type){
        case 1:
            LUTRAM_amount = parallel_RAMs * series_RAMs;
            break;
        case 2:
            BRAM8K_amount = parallel_RAMs * series_RAMs;
            break;
        case 3:
            BRAM128K_amount = parallel_RAMs * series_RAMs;
            break;
        case 4:
            BRAM_amount = parallel_RAMs * series_RAMs;
            break;
        case 5:
            BRAM_amount = parallel_RAMs * series_RAMs;
            break;
    }
    vector<long long> blocks_needed = {LUTRAM_amount, BRAM8K_amount, BRAM128K_amount, BRAM_amount, additional_LUTs_needed};
    return blocks_needed;

}

vector<long long> RAM::mapBRAMS3(int arch, int size, int width, int ratio, long long LUTS, long long BRAM_8KS, long long BRAM_128KS, long long BRAMS, long long add_LUTS, int num_logic_blocks){

    LUT_blocks += LUTS;
    BRAM_8K += BRAM_8KS;
    BRAM_128K += BRAM_128KS;
    BRAM += BRAMS;
    additional_LUTs += add_LUTS;

    switch (arch) {
        case 1:
            if(logical_ram_mode != LogicalRamModes::TrueDualPort) mapBRAM3(arch, LUTRAM, 640, 20, 1, LUTS, BRAM_8KS, BRAM_128KS, BRAMS, add_LUTS, num_logic_blocks);
            mapBRAM3(arch, BRAM8K, 8192, 32, 10, LUTS, BRAM_8KS, BRAM_128KS, BRAMS, add_LUTS, num_logic_blocks);
            mapBRAM3(arch, BRAM128K, 131072, 128, 300, LUTS, BRAM_8KS, BRAM_128KS, BRAMS, add_LUTS, num_logic_blocks);
            break;
        case 2:
            mapBRAM3(arch, BRAM_NOLUT, size, width, ratio, LUTS, BRAM_8KS, BRAM_128KS, BRAMS, add_LUTS, num_logic_blocks);
            break;
        case 3:
            if(logical_ram_mode != LogicalRamModes::TrueDualPort) mapBRAM3(arch, LUTRAM, 640, 20, 1, LUTS, BRAM_8KS, BRAM_128KS, BRAMS, add_LUTS, num_logic_blocks);
            mapBRAM3(arch, BRAM_WITHLUT, size, width, ratio, LUTS, BRAM_8KS, BRAM_128KS, BRAMS, add_LUTS, num_logic_blocks);
            break;
        default:
            break;
    }

    long long LUTRAM_amount = 0;
    long long BRAM8K_amount = 0;
    long long BRAM128K_amount = 0;
    long long BRAM_amount = 0;

    switch (BRAM_type){
        case 1:
            LUTRAM_amount = parallel_RAMs * series_RAMs;
            break;
        case 2:
            BRAM8K_amount = parallel_RAMs * series_RAMs;
            break;
        case 3:
            BRAM128K_amount = parallel_RAMs * series_RAMs;
            break;
        case 4:
            BRAM_amount = parallel_RAMs * series_RAMs;
            break;
        case 5:
            BRAM_amount = parallel_RAMs * series_RAMs;
            break;
    }
    vector<long long> blocks_needed = {LUTRAM_amount, BRAM8K_amount, BRAM128K_amount, BRAM_amount, additional_LUTs_needed};
    return blocks_needed;

}

void RAM::mapBRAM3(int arch, BRAMs bram_type, int size, int max_width, int ratio, long long LUTS, long long BRAM_8KS, long long BRAM_128KS, long long BRAMS, long long add_LUTS, int num_logic_blocks){

    long long cur_LUTRAM_amount = 0;
    long long cur_BRAM8K_amount = 0;
    long long cur_BRAM128K_amount = 0;
    long long cur_BRAM_amount = 0;
    long long cur_additional_LUT_amount = 0;

    

    //loop through all possible width and depth configuration
    //calc max_width available for this logical RAM type
    if(logical_ram_mode == LogicalRamModes::TrueDualPort) max_width = max_width / 2;
    
    int bram_size = size;
    long int bram_area;
    int min_width;

    if(bram_type == LUTRAM){
        bram_area = 40000;
        min_width = 10;
    }
    else{
        bram_area = 9000 + 5*bram_size + 90*sqrt(bram_size) + 1200*max_width;
        min_width = 1;
    }

    //variables needed to keep track of mapping
    long int cur_area;
    int depth;
    int S = 1;
    int P;
    long int muxes = 0;
    int mux_count;
    int decoders = 0;
    long int extra_LUTs = 0;
    int extra_logic_blocks = 0;
    int invalid_mapping = 0;

    //loop through all possible width and depth configuration
    for(int width = min_width; width <= max_width; width = width * 2 ){
        depth = bram_size / width;

        //if need to increase depth of RAM, put blocks in Series
        if(logical_ram_depth > depth){

            //Calc how many blocks in Series (S)
            S = calcPhysicalBlocks(logical_ram_depth, depth);

            //only map if 16 or less blocks in series needed
            if(S <= 16){

                //Calc extra logic needed due to Series blocks
                if(S > 2) decoders = S;
                else decoders = 1;
                
                //calculating total muxes needed (extra)
                if(0){
                    muxes = logical_ram_width;
                }
                else{
                    mux_count = logical_ram_width;
                    int num_luts;
                    int b;
                    for(int row = mux_count; row >=1; row--){
                        num_luts = (S + 5)/6;
                        b = num_luts*(num_luts+1)/2;
                        muxes += row  *  (b*(b+1)/2);
                    }
                }

                extra_LUTs = decoders + muxes;
                if(logical_ram_mode == LogicalRamModes::TrueDualPort) extra_LUTs = extra_LUTs * 2;
                
                extra_logic_blocks = extra_LUTs / 10;
                if(extra_LUTs % 10) extra_logic_blocks += 1;

            }
            else{
                invalid_mapping = 1;
                continue;
            }
        }

        //Calc how many blocks in Parallel (P)
        P = calcPhysicalBlocks(logical_ram_width, width);
        
        //Calc area, set to best area (if smallest so far)
        cur_additional_LUT_amount = extra_LUTs;
        switch (bram_type){
        case 1:
            cur_LUTRAM_amount = P * S;
            break;
        case 2:
            cur_BRAM8K_amount = P * S;
            break;
        case 3:
            cur_BRAM128K_amount = P * S;
            break;
        case 4:
            cur_BRAM_amount = P * S;
            break;
        case 5:
            cur_BRAM_amount = P * S;
            break;
        }

        cur_area = calcFPGAArea(arch, bram_size, width, ratio, cur_LUTRAM_amount + LUT_blocks,
                                                               cur_BRAM8K_amount + BRAM_8K,
                                                               cur_BRAM128K_amount + BRAM_128K,
                                                               cur_BRAM_amount + BRAM,
                                                               cur_additional_LUT_amount + additional_LUTs,
                                                               num_logic_blocks);

        if( (ram_area == 0 || cur_area <= ram_area) && !invalid_mapping ){
            saveRamMapping(extra_LUTs, logical_ram_id, P, S, bram_type, width, depth, cur_area);

        }

    }

}

long double RAM::calcFPGAArea(int arch, int size, int width, int ratio, long long LUTS, long long BRAM_8KS, long long BRAM_128KS, long long BRAMS, long long add_LUTS, int num_logic_blocks){

    long double area;

    if(arch == 1){

        int LUTRAM_ratio = 1;
        int BRAM_8K_ratio = 10;
        int BRAM_128K_ratio = 300;
        
        long long extra_logic_blocks = add_LUTS / 10;
        if(add_LUTS % 10) extra_logic_blocks += 1;

        long long LUT_logic_blocks = LUTS * LUTRAM_ratio;
        long long BRAM_8K_logic_blocks = BRAM_8KS * BRAM_8K_ratio;
        long long BRAM_128K_logic_blocks = BRAM_128KS * BRAM_128K_ratio;
        long long total_logic_blocks_required = extra_logic_blocks + num_logic_blocks + LUT_logic_blocks;

        vector<long long> logic_blocks = {LUT_logic_blocks, BRAM_8K_logic_blocks, BRAM_128K_logic_blocks, total_logic_blocks_required};
        long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

        int num_8K_BRAMs = limiting_factor / BRAM_8K_ratio;
        int num_128K_BRAMs = limiting_factor / BRAM_128K_ratio;

        long long area_LBs = limiting_factor * ( (LUT_logic_blocks != 0) ? 37500 : 35000 );
        long long area_8K = num_8K_BRAMs * calcRamArea(8192, 32);
        long long area_128K = num_128K_BRAMs * calcRamArea(131072, 128);

        area = area_LBs + area_8K + area_128K;

    }
    else if(arch == 2){

        long long extra_logic_blocks = add_LUTS / 10;
        if(add_LUTS % 10) extra_logic_blocks += 1;
        
        long long BRAM_logic_blocks = BRAMS * ratio;
        long long total_logic_blocks_required = extra_logic_blocks + num_logic_blocks;

        vector<long long> logic_blocks = {BRAM_logic_blocks, total_logic_blocks_required};
        long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

        int num_BRAMs = limiting_factor / ratio;

        long long area_LBs = limiting_factor * 35000;
        long long area_BRAM = num_BRAMs * calcRamArea(size, width);

        area = area_LBs + area_BRAM;

    }
    else if(arch == 3){

        int LUTRAM_ratio = 1;
        
        long long extra_logic_blocks = add_LUTS / 10;
        if(add_LUTS % 10) extra_logic_blocks += 1;
        
        long long LUT_logic_blocks = LUTS * LUTRAM_ratio;
        long long BRAM_logic_blocks = BRAMS * ratio;
        long long total_logic_blocks_required = extra_logic_blocks + num_logic_blocks + LUT_logic_blocks;

        vector<long long> logic_blocks = {LUT_logic_blocks, BRAM_logic_blocks, total_logic_blocks_required};
        long long limiting_factor = *max_element(logic_blocks.begin(), logic_blocks.end());

        int num_BRAMs = limiting_factor / ratio;

        long long area_LBs = limiting_factor * ( (LUT_logic_blocks != 0) ? 37500 : 35000 );
        long long area_BRAM = num_BRAMs * calcRamArea(size, width);

        area = area_LBs + area_BRAM;

    }
    else if(arch == 4){

    }

    return area;

}

long long RAM::calcRamArea(int size, int max_width){
    long long area = 9000 + 5*size + 90*sqrt(size) + 1200*max_width;
    return area;
}

void RAM::mapBRAM2(BRAMs bram_type, int size, int max_width, int ratio){

    //loop through all possible width and depth configuration
    //calc max_width available for this logical RAM type
    if(logical_ram_mode == LogicalRamModes::TrueDualPort) max_width = max_width / 2;
    
    int bram_size = size;
    long int bram_area;
    int min_width;

    if(bram_type == LUTRAM){
        bram_area = 40000;
        min_width = 10;
    }
    else{
        bram_area = 9000 + 5*bram_size + 90*sqrt(bram_size) + 1200*max_width;
        min_width = 1;
    }

    //variables needed to keep track of mapping
    long int cur_area;
    int depth;
    int S = 1;
    int P;
    long int muxes = 0;
    int mux_count;
    int decoders = 0;
    long int extra_LUTs = 0;
    int extra_logic_blocks = 0;
    int invalid_mapping = 0;

    //loop through all possible width and depth configuration
    for(int width = min_width; width <= max_width; width = width * 2 ){
        depth = bram_size / width;

        //if need to increase depth of RAM, put blocks in Series
        if(logical_ram_depth > depth){

            //Calc how many blocks in Series (S)
            S = calcPhysicalBlocks(logical_ram_depth, depth);

            //only map if 16 or less blocks in series needed
            if(S <= 16){

                //Calc extra logic needed due to Series blocks
                if(S > 2) decoders = S;
                else decoders = 1;
                
                //calculating total muxes needed (extra)
                if(bram_type == LUTRAM){
                    muxes = logical_ram_width;
                }
                else{
                    mux_count = logical_ram_width;
                    int num_luts;
                    int b;
                    for(int row = mux_count; row >=1; row--){
                        num_luts = (S + 5)/6;
                        b = num_luts*(num_luts+1)/2;
                        muxes += row  *  (b*(b+1)/2);
                    }
                }

                extra_LUTs = decoders + muxes;
                if(logical_ram_mode == LogicalRamModes::TrueDualPort) extra_LUTs = extra_LUTs * 2;
                
                extra_logic_blocks = extra_LUTs / 10;
                if(extra_LUTs % 10) extra_logic_blocks += 1;

            }
            else{
                invalid_mapping = 1;
                continue;
            }
        }

        //Calc how many blocks in Parallel (P)
        P = calcPhysicalBlocks(logical_ram_width, width);
        
        //Calc area, set to best area (if smallest so far)
        cur_area = S * P * bram_area + extra_logic_blocks * 35000;

        if( (ram_area == 0 || cur_area < ram_area) && !invalid_mapping ){
            saveRamMapping(extra_LUTs, logical_ram_id, P, S, bram_type, width, depth, cur_area);
        }

    }

}

int RAM::calcPhysicalBlocks(int logical_length, int physical_length){

    int num_blocks_needed = logical_length / physical_length;
    if(logical_length % physical_length) num_blocks_needed += 1;

    return num_blocks_needed;

}

void RAM::saveRamMapping(long int additional_LUTs, int phys_ram_id, int p, int s, BRAMs ram_type, int phys_width, int phys_depth, int area){

    additional_LUTs_needed = additional_LUTs;
    physical_ram_id = phys_ram_id;
    parallel_RAMs = p;
    series_RAMs = s;

    BRAM_type = ram_type;

    physical_width = phys_width;
    physical_depth = phys_depth;
    ram_area = area;

}

void RAM::clearMapping(){

    additional_LUTs_needed = 0;
    physical_ram_id = 0;
    parallel_RAMs = 0;
    series_RAMs = 0;
    BRAM_type = LUTRAM;
    physical_width = 0;
    physical_depth = 0;
    ram_area = 0;

}

void RAM::printRamMapping(ofstream& mapping_file){

    int bram_type;
    switch(BRAM_type){
        case LUTRAM:
            bram_type = 1;
            break;
        case BRAM8K:
            bram_type = 2;
            break;
        case BRAM128K:
            bram_type = 3;
            break;
        case BRAM_NOLUT:
            bram_type = 1;
            break;
        case BRAM_WITHLUT:
            bram_type = 2;
            break;
    }

    mapping_file << logical_ram_id << " "
                 << additional_LUTs_needed << " "
                 << "LW " << logical_ram_width << " "
                 << "LD " << logical_ram_depth << " "
                 << "ID " << physical_ram_id << " "
                 << "S "  << series_RAMs << " "
                 << "P "  << parallel_RAMs << " "
                 << "Type " << bram_type << " "
                 << "Mode " << romToString(logical_ram_mode) << " "
                 << "W "  << physical_width << " "
                 << "D "  << physical_depth << endl;
}

void RAM::printLogicalRAMs(){

    cout << logical_ram_id << "   " << logical_ram_mode << "   " << logical_ram_depth << "    " << logical_ram_width << endl;
    
}
