
CXX = g++

CXXFLAGS = -Wall -g

OPENMPFLAG = -fopenmp

memorymapper.o : memory_mapping_reader.cpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) -c memory_mapping_reader.cpp -o memorymapper.o

filereader.o : file_reader.cpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) -c file_reader.cpp -o filereader.o

main: memorymapper.o headers/memory_mapping_reader.hpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) main.cpp -o reader memorymapper.o

test: memorymapper.o filereader.o headers/memory_mapping_reader.hpp headers/file_reader.hpp
	$(CXX) $(CXXFLAGS) $(OPENMPFLAG) unit_test.cpp -o test memorymapper.o filereader.o

clean:
	rm memorymapper.o filereader.o reader test