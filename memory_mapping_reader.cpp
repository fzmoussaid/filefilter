#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h> 

#include "headers/memory_mapping_reader.hpp"

uint32_t get_inf_values( data_pair* data, uint32_t data_size,  uint32_t k_largest, uint32_t* res) {
    uint32_t j = 0;
    for (data_pair* ptr_i = data; ptr_i < data + data_size ; ptr_i++){
        if ((*ptr_i).value >= k_largest && j < sizeof(res) / sizeof(uint32_t)){
            *(res + j) = (*ptr_i).id;  
            ++j;
        } else if (j >= sizeof(res) / sizeof(uint32_t)) {
            return 0;
        }
    }
    return 0;
}

// look-up the k-th largest value in an array using the selection algorithm -> complexity on time : linear
uint32_t selection( data_pair *data, uint32_t data_size, uint32_t k ) { 
    uint32_t minIndex, minValue;
   // uint32_t nb_pairs = trunc(sizeof(data) / ( 2 * sizeof(uint32_t)));
    data_pair* tmp_pair = (data_pair*)malloc( 2 * sizeof(uint32_t));
    for (data_pair* ptr_i = data; ptr_i < data + k ; ptr_i++){
        minValue = (*ptr_i).value;
        minIndex = data - ptr_i;
        for (data_pair* ptr_j = ptr_i + 1; ptr_j < data + data_size ; ptr_j++){
            if ( (*ptr_j).value < minValue ) {
                minIndex = data - ptr_j;
                minValue = (*ptr_j).value;
                *tmp_pair = *ptr_i;
                *ptr_i = *(data + minIndex);
                *(data + minIndex) = *tmp_pair;
            }
        }
    }
    free(tmp_pair);

    return (*(data + k)).value;
}

uint32_t nb_occurences(char* s, char c, uint32_t nb) { 
    uint32_t res = 0; 
  
    for (char* i = s; i <  s + nb; i++) {
        if (*(i) == c) {
            res++;
        }
    }           
  
    return res; 
} 

void memory_mapper(const char* path, data_pair* data, uint32_t* data_size) {
    int fd = open(path, O_RDONLY);
    size_t pagesize = getpagesize();
    data_pair* ptr;
    uint32_t pos, nb_lines = 0;
    auto file_size = lseek(fd, 0, SEEK_END);
    std::cout << "Mapping Region Started ..."; 
    char* region = reinterpret_cast<char*>(mmap(NULL, file_size, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0));
    std::cout << "Mapping Region Ended.";
    nb_lines = nb_occurences( region, '\n' , file_size);
    data = (data_pair*) malloc( nb_lines * 2 * sizeof(uint32_t)); // allocating memory at once is more performant
    ptr = data;
    char* datalines = strtok(region, "\n");
    (*data_size) = 0;
    while (datalines != NULL) { 
        std::string line(datalines);
        pos = line.find( ' ' );
        (*ptr).id = std::stoi( line.substr( 0, pos ) );
        (*ptr).value = std::stoi( line.substr( pos) );
        datalines = strtok(NULL, "\n"); 
        ++ptr;
        ++(*data_size);
    }
    
    munmap(region, pagesize);
  
}

