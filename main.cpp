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
    //      3 -> LUTRAM and 1 type of BRAM
    //      4 -> Custom Arch

    RamMapper mapper;
    mapper.parseBenchmarkCircuits(argv[1], argv[2]);
    //mapper.printAllCircuits();
    mapper.mapPhysicalRAM(atoi(argv[4]));
    mapper.genMappingFile(argv[3]);
    //cout << "Geometric Average: " << scientific << mapper.calcGeoAverage() << endl;

    return 0;

}
