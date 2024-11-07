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


    vector<int> block_count;
    int LUT_blocks_used = 0;
    int BRAM_8K_used = 0;
    int BRAM_128K_used = 0;
    long int additional_LUTs = 0;

    int regular_LBs_used = 0;
    int required_LBs = 0;
    long long total_FPGA_area = 0;


public:
    //init and del functions
    Circuit();
    ~Circuit();

    void setCircuitID(int id);
    int getCircuitID();
    void setLogicBlocks(int amount);
    void insertLogicalRAM(int id, string mode, int depth, int width);
    void mapBRAMS(int arch);

    void calcTotalArea();
    long long getCircuitArea();
    
    void printCircuitMapping(ofstream& mapping_file);
    void printCircuit();


};