#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <math.h>
#include <cstdlib>

#include "RamMapper.h"
using namespace std;

int main(int argc, char *argv[]) {

    //to run the mapper and generate a mapping file, run command:
    //  ./RAM_mapper logical_rams.txt logic_block_count.txt mapping.txt <Map Arch>
    //
    //Specify <Map Arch> as int for desired architecture:
    //      1 -> Stratix-IV-Like Arch
    //      2 -> No LUTRAM, 1 type of BRAM
    //          a) Can add following arguments if you want to specify BRAM: <size> <max_width> <ratio>
    //      3 -> LUTRAM and 1 type of BRAM
    //          a) Can add following arguments if you want to specify BRAM: <size> <max_width> <ratio>
    //      4 -> Custom Arch

    //Specify <gen table> if you want the RamMapper to run and generate Table 2 or 3 (experimenting with BRAM size)
    //      table -> generate Table 2 or 3
    int size, max_width, ratio;
    int gen_table;

    if(argc == 8){
        size = atoi(argv[5]);
        max_width = atoi(argv[6]);
        ratio = atoi(argv[7]);
        gen_table = 0;
    }
    else if(argc == 6){
        gen_table = 1;
    }
    else{
        size = 0;
        max_width = 0;
        ratio = 0;
        gen_table = 0;
    }


    RamMapper mapper;
    mapper.parseBenchmarkCircuits(argv[1], argv[2]);
    mapper.mapPhysicalRAM(atoi(argv[4]), size, max_width, ratio, gen_table);
    mapper.genMappingFile(argv[3]);

    return 0;

}
