#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <cstring>
#include <cstdlib>

#include <vector>
#include <bits/stdc++.h>
#include <string>
#include <cmath>

using namespace std;

class RAM{

    enum LogicalRamModes { ROM, SinglePort, SimpleDualPort, TrueDualPort };
    enum BRAMs { LUTRAM=1, BRAM8K, BRAM128K, BRAM_NOLUT, BRAM_WITHLUT };

private:

    //logical RAM data members
    int logical_ram_id;
    LogicalRamModes logical_ram_mode;
    int logical_ram_depth;
    int logical_ram_width;

    //physical RAM data members (BRAM)
    BRAMs BRAM_type;
    long int additional_LUTs_needed;
    int physical_ram_id;
    int parallel_RAMs;
    int series_RAMs;
    int physical_width;
    int physical_depth;
    long int ram_area = 0;

    //variables to keep track of smallest global board area
    long long LUT_blocks = 0;
    long long BRAM_8K = 0;
    long long BRAM_128K = 0;
    long long BRAM = 0;
    long long additional_LUTs = 0;
    long double total_FPGA_area = 0.0;


public:
    //init and del functions
    RAM();
    ~RAM();

    void setLogicalRAM(int id, string mode, int depth, int width);
    string romToString(LogicalRamModes mode);
    vector<long long> mapBRAMS2(int arch, int size, int width, int ratio);
    vector<long long> mapBRAMS3(int arch, int size, int width, int ratio, long long LUTS, long long BRAM_8KS, long long BRAM_128KS, long long BRAMS, long long add_LUTS, int num_logic_blocks);
    int  calcPhysicalBlocks(int logical_length, int physical_length);
    void saveRamMapping(long int additional_LUTs, int phys_ram_id, int p, int s, BRAMs ram_type, int phys_width, int phys_depth, int area);
    void clearMapping();
    void mapBRAM2(BRAMs bram_type, int size, int max_width, int ratio);
    void mapBRAM3(int arch, BRAMs bram_type, int size, int max_width, int ratio, long long LUTS, long long BRAM_8KS, long long BRAM_128KS, long long BRAMS, long long add_LUTS, int num_logic_blocks);
    long double calcFPGAArea(int arch, int size, int width, int ratio, long long LUTS, long long BRAM_8KS, long long BRAM_128KS, long long BRAMS, long long add_LUTS, int num_logic_blocks);
    long long calcRamArea(int size, int max_width);
    void printRamMapping(ofstream& mapping_file);
    void printLogicalRAMs();

};
