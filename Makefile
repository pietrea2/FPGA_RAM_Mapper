all:
	g++ -Wall -c -o main.o main.cpp
	g++ -Wall -c -o RamMapper.o RamMapper.cpp
	g++ -Wall -c -o Circuit.o Circuit.cpp
	g++ -Wall -c -o RAM.o RAM.cpp
	g++ -o RAM_mapper main.o RamMapper.o Circuit.o RAM.o



clean:
	rm -f *.o RAM_mapper
