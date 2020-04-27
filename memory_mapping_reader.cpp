#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h> 

#include "headers/memory_mapping_reader.hpp"

uint32_t get_inf_values( data_pair* data, uint32_t data_size,  uint32_t k_largest, uint32_t* res, uint32_t nb_instances) {
    uint32_t j = 0;
    for (data_pair* ptr_i = data; ptr_i < data + data_size ; ptr_i++){
        if ((*ptr_i).value >= k_largest && j < nb_instances){
            *(res + j) = (*ptr_i).id;  
            ++j;
        } else if (j == nb_instances) {
            return 0;
        }
    }
    return 0;
}

// look-up the k-th largest value in an array using the selection algorithm -> complexity on time : linear
uint32_t selection( data_pair *data, uint32_t data_size, uint32_t k ) { 
    uint32_t maxValue;
    data_pair* tmp_pair = (data_pair*)malloc( 2 * sizeof(uint32_t));
    for (data_pair* ptr_i = data; ptr_i < data + k ; ptr_i++){
        maxValue = (*ptr_i).value;
        // std::cout << maxValue << std::endl;
        for (data_pair* ptr_j = ptr_i + 1; ptr_j < data + data_size ; ptr_j++){
            if ( (*ptr_j).value >= maxValue ) {
                maxValue = (*ptr_j).value;
                *tmp_pair = *ptr_i;
                *ptr_i = *ptr_j;
                *ptr_j = *tmp_pair;
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

void get_data_pairs(char* s, data_pair *data, uint32_t* data_size) {
    data_pair *ptr;
    char *pos_pointer, *id_value;
    char *id, *value;

    ptr = data;
    pos_pointer = strchr(s, '\n' );
    while( pos_pointer != NULL ) {
        // std::cout << "line position " << line_delimiter_pos << '\n';
        id_value = strchr(s, ' ' );
        // std::cout << "sep position " << id_value - s << '\n';
        id = (char*)(malloc(id_value - s));
        memcpy(id, s, id_value - s);
        value = (char*)(malloc(pos_pointer - id_value));
        memcpy(value, id_value, pos_pointer - id_value);

        (*ptr).id = std::stoi( id );
        (*ptr).value = std::stoi( value );
        // std::cout << "ID : " << (*ptr).id  << std::endl;
        // std::cout << "Value : " << (*ptr).value  << std::endl;
        ++ptr;
        ++(*data_size);
        s = pos_pointer + 1;
        pos_pointer = strchr(s, '\n' );
    }

}

data_pair* memory_mapper(const char* path, uint32_t* data_size) {
    int fd = open(path, O_RDONLY);
    size_t pagesize = getpagesize();
    
    uint32_t nb_lines = 0;
    data_pair* data;
    auto file_size = lseek(fd, 0, SEEK_END);
    uint32_t nb_pages = (file_size % pagesize == 0)? file_size / pagesize : file_size / pagesize + 1;
    // std::cout << "File Size :" << file_size << '\n'; 
    // std::cout << "Nb Pages :" << nb_pages << '\n'; 
    // std::cout << "Page size :" << pagesize << '\n'; 


    std::cout << "Mapping Region Started ..." << '\n';  
    char* region = (char*)(mmap(NULL, nb_pages * pagesize, PROT_READ, MAP_SHARED, fd, 0));
    if (region == MAP_FAILED) {
       perror("Could not mmap");
       return NULL;
    }
    // std::cout << "Mapping Region Ended." << '\n';    
    nb_lines = nb_occurences( region, '\n' , file_size);
    // std::cout << nb_lines << '\n';  
    data = (data_pair*) malloc( nb_lines * 2 * sizeof(uint32_t)); // allocating memory at once is more performant

    get_data_pairs(region, data, data_size);
    
    munmap(region, pagesize);

    return data;
  
}

