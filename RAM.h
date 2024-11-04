#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <cstring>
#include <cstdlib>

#include <vector>

using namespace std;

class RAM{

    enum LogicalRamModes { ROM, SinglePort, SimpleDualPort, TrueDualPort };
    enum BRAMs { LUTRAM, BRAM8K, BRAM128K };

private:

    //logical RAM data members
    int logical_ram_id;
    LogicalRamModes logical_ram_mode;
    int logical_ram_depth;
    int logical_ram_width;

    //physical RAM data members (BRAM)
    int additional_LUTs_needed;
    int physical_ram_id;
    int parallel_RAMs;
    int series_RAMs;
    int RAM_type;
    int physical_width;
    int physical_depth;

    long int ram_area;
    





public:
    //init and del functions
    RAM();
    ~RAM();

    void setLogicalRAM(int id, string mode, int depth, int width);
    void mapBRAMS(int arch);
    void mapLUTRAM();
    void mapBRAM8K(int arch);
    void mapBRAM128K(int arch);

    long int calcRamArea(int LUTRAM_support, int bits, int max_width);

    void printLogicalRAMs();


};