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

using namespace std;

class RamMapper{
private:
    //vector of circuits
    vector<Circuit> circuit_array;
    vector<long int> circuit_areas;

public:

    //init and del functions
    RamMapper();
    ~RamMapper();
    void parseBenchmarkCircuits(char *logicalRAMsList, char * logicalBlockCount);
    void mapPhysicalRAM(int architecture);

    void printAllCircuits();




};
