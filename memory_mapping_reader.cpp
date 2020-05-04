#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h> 
#include <sys/sysinfo.h>
#include <omp.h>
#include <assert.h> 

#include "headers/memory_mapping_reader.hpp"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void get_data_pairs(char* s, data_pair *data, uint16_t* data_size, uint32_t nb_lines, char* last_element, uint16_t section) {
    
    data_pair *ptr;
    char *pos_pointer, *id_value;
    char *id, *value;
    assert(nb_lines != 0);
    uint32_t i = 1;
    int8_t nb_proc = get_nprocs();
    uint32_t nb_total = 0;
    for(int i = 0; i < nb_proc ; i++) 
        *(data_size + i) = 0;
    ptr = data;
    if (section != 0) {
        pos_pointer = strchr(s, '\n' );
        s = pos_pointer + 1;
    }
    pos_pointer = strchr(s, '\n' );


    while( pos_pointer < last_element && pos_pointer != NULL) {
        if ( nb_total >  i * nb_lines / nb_proc) {
            i++;
        }
        id_value = strchr(s, ' ' );
        id = (char*)(malloc(id_value - s + 1));
        memcpy(id, s, id_value - s + 1);
        if ((int)(pos_pointer - id_value)  < 0) {
            std::cout << "point 3-2 : " << (int)(pos_pointer - id_value) << " end" << std::endl;    
        }
        value = (char*)(malloc(pos_pointer - id_value + 1));
        memcpy(value, id_value, pos_pointer - id_value + 1);

        (*ptr).id = std::strtol( id , NULL, 10);
        (*ptr).value = std::strtol( value , NULL, 10);
        ++ptr;
        ++(*(data_size + i - 1));
        ++nb_total;
        s = pos_pointer + 1;
        pos_pointer = strchr(s, '\n' );
        free(id);
        free(value);

    }


}

data_pair* memory_mapping( int* fd, data_pair* res, uint32_t nb_instances) {
    off64_t file_size = lseek(*fd, 0, SEEK_END);
    char* region = (char*)(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, *fd, 0));
    if (region == MAP_FAILED) {
       perror("Could not mmap");
       return NULL;
    }

    get_k_max(region, file_size, res, nb_instances);
    
    munmap(region, file_size );
    
    return res;
  
}

void get_k_max( char* region,  uint64_t data_size, data_pair *res, uint32_t nb_instances) {
    data_pair *ptr = (data_pair*) malloc( 2 * sizeof(uint64_t) );
    char *pos_pointer, *id_value;
    char *id, *value;
    pos_pointer = strchr(region, '\n' );


    while( pos_pointer < region + data_size && pos_pointer != NULL) {
        id_value = strchr(region, ' ' );
        id = (char*)(malloc(id_value - region + 1));
        memcpy(id, region, id_value - region + 1);

        value = (char*)(malloc(pos_pointer - id_value + 1));
        memcpy(value, id_value, pos_pointer - id_value + 1);
        
        (*ptr).id = std::strtol( id , NULL, 10);
        (*ptr).value = std::strtol( value , NULL, 10);
        n_max_val( res, ptr, nb_instances );

        region = pos_pointer + 1;
        pos_pointer = strchr(region, '\n' );
        free(id);
        free(value);
    }
}

void filter_section(const char* path, uint32_t nb_instances, data_pair* res_pair ) {
    int fd = open(path, O_RDONLY);
    uint64_t file_size = lseek(fd, 0, SEEK_END);
    std::cout << "File Size is " << file_size << std::endl;
    res_pair = memory_mapping( &fd, res_pair, nb_instances);   
    close(fd);
}


void n_max_val_section( data_pair *res, uint32_t nb_instances, data_pair *pairs ,  uint16_t data_size) {  
    for (data_pair* ptr = pairs; ptr < pairs + data_size; ptr++){ 
        // std::cout << "data : " << (*(ptr)).id  << std::endl;
        n_max_val( res, ptr, nb_instances );
    }

}

void n_max_val( data_pair *pairs, data_pair* new_val, uint32_t nb_instances ) {  
   data_pair* res;
   res = pairs; 
   for (data_pair* ptr = pairs; ptr < pairs + nb_instances; ptr++){ 
      if ( (*ptr).value < (*res).value ) {
         res = ptr;
      }
   }

   if ( (*res).value < (*new_val).value ) {
      (*res).value = (*new_val).value;
      (*res).id = (*new_val).id;
   }

}

// look-up the k-th largest value in an array using the selection algorithm -> complexity on time : O(n^2) worst case scenario
uint32_t selection( data_pair *data, uint32_t data_size, uint32_t k ) { 
    uint32_t maxValue;
    data_pair* tmp_pair = (data_pair*)malloc( 2 * sizeof(uint32_t));
    for (data_pair* ptr_i = data; ptr_i < data + k ; ptr_i++){
        maxValue = (*ptr_i).value;
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

uint32_t get_inf_values( data_pair* data, uint16_t data_size,  uint32_t k_largest, uint32_t* res, uint16_t nb_instances) {
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

uint32_t nb_occurences(char* s, char c, char* last_element) { 
    uint32_t res = 0; 
    char *pos_pointer;
    pos_pointer = strchr(s, '\n' );
    while ( pos_pointer < last_element && pos_pointer != NULL) {
        res++;    
        s = pos_pointer + 1;
        pos_pointer = strchr(s, '\n' );
    }    
  
    return res; 
} 
