#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <cstring>
#include <cstdlib>

#include "Circuit.h"
#include <vector>
#include <string>
#include <cmath>

using namespace std;

class RamMapper{
private:
    //vector of circuits
    vector<Circuit> circuit_array;
    //vector<long long> circuit_areas;
    long double geo_ave_area;

public:

    //init and del functions
    RamMapper();
    ~RamMapper();
    void parseBenchmarkCircuits(char *logicalRAMsList, char * logicalBlockCount);
    void mapPhysicalRAM(int architecture, int bram_size, int max_width, int bram_ratio, int generate_table);
    void genMappingFile(char *mapping_file);

    long double calcGeoAverage();

    void printAllCircuits();




};
