# AssemblerSimulator
Realized an assembler and simulator, written 3 programs in assembly for RISC processor ( a simpler version of MIPS processor)
assembler module translates assembly language into machine code, the machine code is then fed as input into the simulator which performs a simulation
of the provided assembly program.

## How to compile and run


### On Windows OS: 
1. download and install MinGW: https://osdn.net/projects/mingw/downloads/68260/mingw-get-setup.exe/
2. download all project files into the same folder, insert the following command: cd /...your_file_path.../
3. insert the following command: gcc assembler.c -o Assembler
4. insert the following command: gcc simulator.c -o Simulator
5. insert the following command to initialize Assembler: Assembler --program="/path_to_program/" --memoryin=/"path_to_memory_in/
6. insert the following command to initialize Simulator: Simultor --memoryin="/path/to/memory_input/" --memoryout="/memory_output_path/"
--regout="/path_to_register_output/" --trace=/path_to_traces/ --count=/path_to_count/


 * memoryin in Assembler and Simulator is the same file, thus has the same path
 * you can choose between 3 assembly programs: 
   pascal.asm - calculate the nth row of a pascal triangle (https://en.wikipedia.org/wiki/Pascal%27s_triangle)
   bubble.asm - sorts an array using bubble sort
   div.asm - calculates x/y and the remainder x%y
    
## IDE
Written and compiled in Visual Studio 2017

## License
[MIT](https://choosealicense.com/licenses/mit/)
