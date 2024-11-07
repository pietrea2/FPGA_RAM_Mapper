#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <cstring>
#include <cstdlib>

#include <vector>
#include <string>
#include <cmath>

using namespace std;

class RAM{

    enum LogicalRamModes { ROM, SinglePort, SimpleDualPort, TrueDualPort };
    enum BRAMs { LUTRAM=1, BRAM8K, BRAM128K };

private:

    //logical RAM data members
    int logical_ram_id;
    LogicalRamModes logical_ram_mode;
    int logical_ram_depth;
    int logical_ram_width;

    //physical RAM data members (BRAM)
    int BRAM_type;
    long int additional_LUTs_needed;
    int physical_ram_id;
    int parallel_RAMs;
    int series_RAMs;
    int physical_width;
    int physical_depth;
    long int ram_area = 0;






public:
    //init and del functions
    RAM();
    ~RAM();

    void setLogicalRAM(int id, string mode, int depth, int width);
    string romToString(LogicalRamModes mode);
    vector<long long> mapBRAMS(int arch);
    vector<long long> mapArch2BRAM(int size, int width, int ratio);
    int  calcPhysicalBlocks(int logical_length, int physical_length);
    void saveRamMapping(long int additional_LUTs, int phys_ram_id, int p, int s, BRAMs ram_type, int phys_width, int phys_depth, int area);
    void clearMapping();
    void mapLUTRAM();
    void mapBRAM8K(int arch);
    void mapBRAM128K(int arch);
    void mapBRAM(int size, int max_width, int ratio);


    void printRamMapping(ofstream& mapping_file);
    void printLogicalRAMs();


};
