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
    float geo_ave_area;

public:

    //init and del functions
    RamMapper();
    ~RamMapper();
    void parseBenchmarkCircuits(char *logicalRAMsList, char * logicalBlockCount);
    void mapPhysicalRAM(int architecture);
    void genMappingFile(char *mapping_file);

    float calcGeoAverage();

    void printAllCircuits();




};
