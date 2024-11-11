#include "RamMapper.h"

using namespace std;

void RamMapper::parseBenchmarkCircuits(char *logicalRAMsList, char * logicalBlockCount){

    //Parse logical block count file
    string input;
    ifstream logicalCountFile;
    logicalCountFile.open(logicalBlockCount);
    if( logicalCountFile.is_open() ){
        getline(logicalCountFile, input);
        //cout << input << endl;
    }
    
    int circ_id;
    int num_logic_blocks;
    while( logicalCountFile >> circ_id >> num_logic_blocks ){
        Circuit circuit;
        circuit.setCircuitID(circ_id);
        circuit.setLogicBlocks(num_logic_blocks);
        circuit_array.push_back(circuit);

    }

    logicalCountFile.close();


    //Parse logical RAM file
    ifstream logicalRamFile;
    logicalRamFile.open(logicalRAMsList);
    if( logicalRamFile.is_open() ){
        //Pass through first 2 lines in file
        getline(logicalRamFile, input);
        //cout << input << endl;
        getline(logicalRamFile, input);
        //cout << input << endl;
    }
    
    int ramID;
    string ramMode;
    int depth, width;
    
    while( logicalRamFile >> circ_id >> ramID >> ramMode >> depth >> width ){
        circuit_array[circ_id].insertLogicalRAM(ramID, ramMode, depth, width);
        circuit_array[circ_id].sort_RAMS();
    }

    logicalRamFile.close();


}

void RamMapper::mapPhysicalRAM(int architecture, int bram_size, int max_width, int bram_ratio, int generate_table){

    if(architecture == 1 ){

        clock_t start = clock();
        for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
            (*i).mapBRAMS2(architecture, 0, 0, 0);
        }
        clock_t finish = clock();
        
        geo_ave_area = calcGeoAverage();
            
        cout << "Geo Average Area: " << scientific << geo_ave_area << endl;
        cout << "CPU Runtime: " << scientific <<  (double)(finish - start)/CLOCKS_PER_SEC << endl;
        
    }
    else if(architecture == 2 || architecture == 3){

        if(generate_table == 0){

            //start looping through all circuits
            clock_t start = clock();
            for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
                (*i).mapBRAMS2(architecture, bram_size, max_width, bram_ratio);
            }
            clock_t finish = clock();
            
            geo_ave_area = calcGeoAverage();
            
            cout << "BRAM size: " << bram_size << "k " << "Max Width: " << max_width << " LB per RAM: " << bram_ratio << " Geo Average Area: " << scientific << geo_ave_area << endl;
            cout << "CPU Runtime: " << scientific <<  (double)(finish - start)/CLOCKS_PER_SEC << endl;

        }
        else{

            //loop for BRAM sizes (1k - 128k)
            long double best_geo_average;
            int best_width, best_ratio;
            long double curr_geo_average;

            int base_size = 1024;
            int max_size = 128;
            for(int size = 1; size <= max_size; size *= 2){

                best_geo_average = 0.0;
                
                //loop for sizes of max_width (8 - 512)
                for(int width = 2; width <= 512; width *= 2){
                    
                    //loop for ratio of LBs to RAM blocks
                    for(int ratio = 1; ratio <= 100; ratio++){

                        //start looping through all circuits
                        for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
                            (*i).mapBRAMS2(architecture, size*base_size, width, ratio);
                        }

                        curr_geo_average = calcGeoAverage();

                        //save smallest geo_average
                        if(best_geo_average == 0.0 || curr_geo_average < best_geo_average){
                            best_geo_average = curr_geo_average;
                            best_width = width;
                            best_ratio = ratio;
                        }

                        //clear best solution variables in circuit
                        for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
                            (*i).clearMapping();
                        }
                        
                    }
                }

                //keep track of all geo averages
                //keep smallest area
                cout << "BRAM size: " << size << "k " << "Max Width: " << best_width << " LB per RAM: " << best_ratio << " Geo Average Area: " << scientific << best_geo_average << endl;
                
            }

        }
        
    }



    

}

void RamMapper::genMappingFile(char *mapping_file){

    string input;
    ofstream  mappingFileStream;
    mappingFileStream.open(mapping_file);

    for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
        (*i).printCircuitMapping(mappingFileStream);
    }

    mappingFileStream.close();
}

long double RamMapper::calcGeoAverage(){

    long double area = 1.0;
    long double root = 69.0;

    for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
        area *= ( (*i).getCircuitArea() / pow(10.0, 7) );
    }

    geo_ave_area = pow( area, 1.0/root);
    geo_ave_area *= pow(10.0, 7);
    return geo_ave_area;

}

void RamMapper::printAllCircuits(){

    // Print vector elements using iterators
    for (auto i = circuit_array.begin(); i != circuit_array.end(); ++i){
        (*i).printCircuit();
    }

}
