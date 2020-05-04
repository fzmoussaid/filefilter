
CXX = g++

CXXFLAGS = -Wall -g

OPENMPFLAG = -fopenmp

memorymapper.o : memory_mapping_reader.cpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) -c memory_mapping_reader.cpp -o memorymapper.o

main: memorymapper.o headers/memory_mapping_reader.hpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) main.cpp -o reader memorymapper.o

