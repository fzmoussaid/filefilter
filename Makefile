all: testgen test memmap_filter benchmark
.PHONY: all

CXX = g++

CXXFLAGS = -Wall -g

OPENMPFLAG = -fopenmp

memorymapper.o : memory_mapping_reader.cpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) -c memory_mapping_reader.cpp -o memorymapper.o

filereader.o : file_reader.cpp
	$(CXX) $(CXXFLAGS) -c file_reader.cpp -o filereader.o

memmap_filter: memorymapper.o headers/memory_mapping_reader.hpp filereader.o headers/file_reader.hpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) main.cpp memorymapper.o filereader.o -o memmap_filter 

testgen:
	$(CXX) $(CXXFLAGS) test_generator.cpp -o testgen

test: memorymapper.o filereader.o headers/memory_mapping_reader.hpp headers/file_reader.hpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) test.cpp -o test memorymapper.o filereader.o

benchmark: memorymapper.o filereader.o headers/memory_mapping_reader.hpp headers/file_reader.hpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) benchmarks.cpp -o benchmark memorymapper.o filereader.o

clean:
	rm -f memorymapper.o filereader.o memmap_filter test testgen benchmark

checkmemleak: memmap_filter
	valgrind --tool=memcheck --leak-check=full -v ./memmap_filter testfiles/sample_file_10500.dat
