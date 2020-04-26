
CXX = g++

CXXFLAGS = -Wall -g

memorymapper.o : memory_mapping_reader.cpp
	$(CXX) $(CXXFLAGS) -c memory_mapping_reader.cpp -o memorymapper.o

main: memorymapper.o headers/memory_mapping_reader.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o reader memorymapper.o
