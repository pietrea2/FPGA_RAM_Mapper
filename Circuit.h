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
#include <bits/stdc++.h>
#include <cmath>

using namespace std;

class Circuit{
private:
    //vector of RAMs
    vector<RAM> ram_array;
    int circuit_id;
    int num_logic_blocks;


    vector<long long> block_count;
    long long LUT_blocks_used = 0;
    long long BRAM_8K_used = 0;
    long long BRAM_128K_used = 0;
    long long BRAM_used = 0;        //for table 2 and 3 (BRAM size exploration)
    int BRAM_1_used = 0;            //for custom architecture in part 4
    int BRAM_2_used = 0;
    long long additional_LUTs = 0;
    long double total_FPGA_area = 0.0;


public:
    //init and del functions
    Circuit();
    ~Circuit();

    void setCircuitID(int id);
    int getCircuitID();
    void setLogicBlocks(int amount);
    void insertLogicalRAM(int id, string mode, int depth, int width);
    void sort_RAMS();

    void mapBRAMS2(int arch, int size, int width, int ratio);
    void mapBRAMS3(int arch, int size, int width, int ratio);
    void clearMapping();

    void calcTotalArea3(int arch, int size, int width, int ratio);
    long double getCircuitArea();
    long long calcRamArea(int size, int max_width);
    
    void printCircuitMapping(ofstream& mapping_file);
    void printCircuit();


};
