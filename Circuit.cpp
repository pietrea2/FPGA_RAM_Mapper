#include "Circuit.h"

using namespace std;

Circuit::Circuit(){

}

Circuit::~Circuit(){

}

void Circuit::setCircuitID(int id){
    circuit_id = id;
}

void Circuit::setLogicBlocks(int amount){
    num_logic_blocks = amount;
}

void Circuit::insertLogicalRAM(){
    
}

void Circuit::mapBRAMS(){

}

void Circuit::printCircuit(){
    cout << circuit_id << "   " << num_logic_blocks << endl;
}
