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
private:

    //logical RAM data members
    int logical_ram_id;
    int logical_ram_mode;
    int logical_ram_depth;
    int logical_ram_width;

    //physical RAM data members (BRAM)




public:
    //init and del functions
    RAM();
    ~RAM();

    void insertLogicalRAM();
    void mapBRAMS();
    void mapLUTRAM();
    void mapBRAM8K();
    void mapBRAM128K();

    enum LogicalRamModes { ROM, SinglePort, SimpleDualPort, TrueDualPort };
    enum BRAMs { LUTRAM, BRAM8K, BRAM128K };


};