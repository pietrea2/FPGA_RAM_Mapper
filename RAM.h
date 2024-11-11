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
    enum BRAMs { LUTRAM=1, BRAM8K, BRAM128K, BRAM_NOLUT, BRAM_WITHLUT, BRAM_1, BRAM_2 };

private:

    //logical RAM data members
    int logical_ram_id;
    LogicalRamModes logical_ram_mode;
    int logical_ram_depth;
    int logical_ram_width;
    int logical_size;

    //physical RAM data members (BRAM)
    BRAMs BRAM_type;
    long int additional_LUTs_needed;
    int physical_ram_id;
    int parallel_RAMs;
    int series_RAMs;
    int physical_width;
    int physical_depth;
    long int ram_area = 0;


public:

    bool operator < (const RAM& obj) const{
        return logical_size > obj.logical_size;
    }

    void setLogicalRAM(int id, string mode, int depth, int width);
    string romToString(LogicalRamModes mode);
    vector<long long> mapBRAMS(int arch, int size, int width, int ratio);

    int  calcPhysicalBlocks(int logical_length, int physical_length);
    void saveRamMapping(long int additional_LUTs, int phys_ram_id, int p, int s, BRAMs ram_type, int phys_width, int phys_depth, long int area);
    void clearMapping();

    void mapBRAM(BRAMs bram_type, int size, int max_width, int ratio);
    void printRamMapping(ofstream& mapping_file);
    void printLogicalRAMs();

};
