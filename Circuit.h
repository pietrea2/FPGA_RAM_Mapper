#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <cstring>
#include <cstdlib>

#include "RAM.h"
#include <vector>

using namespace std;

class Circuit{
private:
    //vector of RAMs
    vector<RAM> ram_array;
    int circuit_id;
    int num_logic_blocks;


    int LUT_blocks_used;
    int BRAM_8K_used;
    int BRAM_128K_used;
    int regular_LBs_used;
    int required_LBs;
    long int total_FPGA_area;


public:
    //init and del functions
    Circuit();
    ~Circuit();

    void setCircuitID(int id);
    void setLogicBlocks(int amount);
    void insertLogicalRAM(int id, string mode, int depth, int width);
    void mapBRAMS(int arch);

    long long calcTotalArea();
    
    void printCircuit();


};