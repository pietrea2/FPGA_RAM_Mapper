# FPGA RAM Mapper by Adam Pietrewicz

CAD software tool that maps logical memories to different physical block RAMs (BRAMs) on various FPGA architectures.
It computes a mapping that minimizes the total FPGA area necessary.
Complete description and evaluation of this mapper can be found in the [report directory](https://github.com/pietrea2/FPGA_RAM_Mapper/tree/main/report).

Once unzipped, this folder contains:
- all C++ source code files
- this README
- Makefile
- mapping.txt file: RAM mapping that passe slegality checker for the fixed Stratix-IV-like architecture
- table1/2/3.txt: table 1, 2, 3 data output (for the report) from running the RAM mapper

How to run RAM Mapper:
1. In terminal, go to this folder and run:
    - make

2. Now we have compiled the C++ code and have an executable: ./RAM_mapper

3. To run the mapper tool for the Stratix-IV-like architecture (1):
    - ./RAM_mapper logical_rams.txt logic_block_count.txt mapping.txt 1

4. This will output the geometric average of the mapping and the CPU runtime of the program.

5. Next, we can run the mapper for architectures with no LUTRAM and one type of block RAM (2):
    - ./RAM_mapper logical_rams.txt logic_block_count.txt mapping.txt 2 <bram size> <max width> <ratio>
    - Example:
        - ./RAM_mapper logical_rams.txt logic_block_count.txt mapping.txt 2 1024 4 2

6. To generate Table 2 (find the best BRAM configuration that gives smallest area mapping) run:
    - ./RAM_mapper logical_rams.txt logic_block_count.txt mapping.txt 2 table

7. Similarly, run the following for architectures with LUTRAM and one type of block RAM (3):
    - ./RAM_mapper logical_rams.txt logic_block_count.txt mapping.txt 3 <bram size> <max width> <ratio>
    - Example:
        - ./RAM_mapper logical_rams.txt logic_block_count.txt mapping.txt 3 1024 4 2
        - ./RAM_mapper logical_rams.txt logic_block_count.txt mapping.txt 3 table

8. Finally, to map RAM using custom architecture (4) run:
    - ./RAM_mapper logical_rams.txt logic_block_count.txt mapping.txt 4

    - this architecture uses the following:
        - BRAM1: 4k bit size, 16 bit max width, 4:1 ratio
        - BRAM2: 64k bit size, 64 bit max width, 64:1 ratio

    
