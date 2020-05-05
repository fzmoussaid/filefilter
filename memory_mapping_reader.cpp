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



// Map the file of the file descriptor fd and return the k largest values with their ids in res
data_pair* memory_mapping( int* fd, data_pair* res, uint32_t nb_instances) {
    off64_t file_size = lseek(*fd, 0, SEEK_END);
    char* region = (char*)(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, *fd, 0));
    if (region == MAP_FAILED) {
       perror(MAPPING_ERROR);
       return NULL;
    }

    get_k_max(region, file_size, res, nb_instances);

    munmap(region, file_size );

    return res;

}

// Map the file of the file descriptor fd and return the k largest values with their ids in res
data_pair* memory_mapping_section( int* fd, data_pair* res, uint32_t nb_instances) {
    off64_t file_size = lseek(*fd, 0, SEEK_END);
    int8_t nb_proc = get_nprocs();
    char* region = (char*)(mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, *fd, 0));
    if (region == MAP_FAILED) {
       perror(MAPPING_ERROR);
       return NULL;
    }
    data_pair* section_res = (data_pair*)calloc(nb_proc * nb_instances, 2 * sizeof(uint32_t));
    uint64_t Nb_sections = nb_proc;
    #pragma omp parallel for
    for (int section = 0; section < Nb_sections; section++){
        
        std::cout << "Section : " << section << std::endl;
        int start_offset = 0, end_offset = 0;
        
        if (section == Nb_sections - 1) {
            while(*(region + section * file_size / 4 + start_offset) != '\n')
                start_offset++;
            get_k_largest_section(region + section * file_size / 4 + start_offset + 1, file_size / 4 + file_size % 4 + start_offset + 1,  section_res + section * nb_instances, nb_instances);
        } else if (section == 0) {
            while(*(region + (section + 1) * file_size / 4 + end_offset) != '\n') {
                end_offset++;
            }
            get_k_largest_section(region + section * file_size / 4, file_size / 4 + end_offset, section_res + section * nb_instances, nb_instances);
        } else {
            while(*(region + (section + 1) * file_size / 4 + end_offset) != '\n') {
                end_offset++;
            }

            while(*(region + section * file_size / 4 + start_offset) != '\n') {
                start_offset++;
            }
            std::cout << "End : " << end_offset << std::endl;
            std::cout << "Start : " << start_offset << std::endl;
            get_k_largest_section(region + section * file_size / 4 + start_offset + 1, file_size / 4 + end_offset + start_offset + 1, section_res + section * nb_instances, nb_instances);
        }
    
    } 
    uint32_t k_largest = selection(section_res, nb_instances * nb_proc, nb_instances );
    get_inf_values( section_res, nb_instances * nb_proc,  k_largest, res, nb_instances);
    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(section_res + i)).id << '\n';
    } 
    
    munmap(region, file_size );

    return res;

}

// Parse the mapped region and return the k largest values with their ids in res
void get_k_largest_section( char* region, uint64_t data_size, data_pair *res, uint32_t k) {
    data_pair *ptr = (data_pair*) malloc( 2 * sizeof(uint32_t) );
    char *pos_pointer, *id_value;
    char *id, *value;
    pos_pointer = strchr(region, '\n' );
    int nb_read_char = 0;
    // while( pos_pointer < region + data_size && pos_pointer != NULL) {
    while( nb_read_char < data_size && pos_pointer != NULL) { 
        id_value = strchr(region, ' ' );
        if(id_value == NULL) {
            std::cout << FILE_BAD_FORMAT_ERROR << std::endl;
            exit (EXIT_FAILURE);
        }
        id = (char*)(malloc(id_value - region + 1));
        memcpy(id, region, id_value - region + 1);
        if((int)(pos_pointer - id_value ) < 0) {
            std::cout << FILE_BAD_FORMAT_ERROR << std::endl;
            exit (EXIT_FAILURE);
        }
        value = (char*)(malloc(pos_pointer - id_value + 1));
        memcpy(value, id_value, pos_pointer - id_value + 1);
        try {
        (*ptr).id = std::strtol( id , NULL, 10);
        (*ptr).value = std::strtol( value , NULL, 10);

        // std::cout << ptr->id << std::endl;
    
        } catch ( std::invalid_argument const &e ) {
            std::cout << ID_VALUE_ERROR << '\n';
            exit (EXIT_FAILURE);
         }
         catch ( std::out_of_range const &e ) {
            std::cout << OVERFLOW_ERROR << '\n';
            exit (EXIT_FAILURE);
         }
        
        n_max_val( res, ptr, k );
        nb_read_char = nb_read_char + (pos_pointer - region);
        region = pos_pointer + 1;
        
        pos_pointer = strchr(region, '\n' );
        free(id);
        free(value);
    }
}

// Parse the mapped region and return the k largest values with their ids in res
void get_k_max( char* region,  uint64_t data_size, data_pair *res, uint32_t k) {
    data_pair *ptr = (data_pair*) malloc( 2 * sizeof(uint32_t) );
    char *pos_pointer, *id_value;
    char *id, *value;
    pos_pointer = strchr(region, '\n' );


    while( pos_pointer < region + data_size && pos_pointer != NULL) {
        id_value = strchr(region, ' ' );
        if(id_value == NULL) {
            std::cout << FILE_BAD_FORMAT_ERROR << std::endl;
            exit (EXIT_FAILURE);
        }
        id = (char*)(malloc(id_value - region + 1));
        memcpy(id, region, id_value - region + 1);
        if((int)(pos_pointer - id_value ) < 0) {
            std::cout << FILE_BAD_FORMAT_ERROR << std::endl;
            exit (EXIT_FAILURE);
        }
        value = (char*)(malloc(pos_pointer - id_value + 1));
        memcpy(value, id_value, pos_pointer - id_value + 1);
        try {
        (*ptr).id = std::strtol( id , NULL, 10);
        (*ptr).value = std::strtol( value , NULL, 10);
    
        } catch ( std::invalid_argument const &e ) {
            std::cout << ID_VALUE_ERROR << '\n';
            exit (EXIT_FAILURE);
         }
         catch ( std::out_of_range const &e ) {
            std::cout << OVERFLOW_ERROR << '\n';
            exit (EXIT_FAILURE);
         }
        
        n_max_val( res, ptr, k );

        region = pos_pointer + 1;
        pos_pointer = strchr(region, '\n' );
        free(id);
        free(value);
    }
}

// Read file or stdin and returns the k largest values with their ids in res_pair
void file_mapping_filter(const char* path, uint32_t k, data_pair* res_pair ) {
    int fd;
    if (strlen(path) == 0)
        fd = STDIN_FILENO;
    else
        fd = open(path, O_RDONLY);
    res_pair = memory_mapping( &fd, res_pair, k);
    close(fd);
}

// Read file or stdin and returns the k largest values with their ids in res_pair
void file_mapping_filter_section(const char* path, uint32_t k, data_pair* res_pair ) {
    int fd;
    if (strlen(path) == 0)
        fd = STDIN_FILENO;
    else
        fd = open(path, O_RDONLY);
    res_pair = memory_mapping_section( &fd, res_pair, k);
    close(fd);
}

// Checks if new value is greater than one of the stored values
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

// look-up the k-th largest value in an array using the selection algorithm -> complexity on time : O(data_size * k) 
uint32_t selection( data_pair *data, uint32_t data_size, uint32_t k ) {
    uint32_t maxValue;
    data_pair* tmp_pair = (data_pair*)malloc( 2 * sizeof(uint32_t));
    for (data_pair* ptr_i = data; ptr_i < data + k ; ptr_i++){
        maxValue = (*ptr_i).value;
        for (data_pair* ptr_j = ptr_i + 1; ptr_j < data + data_size ; ptr_j++) {
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

uint32_t get_inf_values( data_pair* data, uint32_t data_size,  uint32_t k_largest, data_pair* res, uint16_t nb_instances) {
    uint32_t j = 0;
    for (data_pair* ptr_i = data; ptr_i < data + data_size ; ptr_i++){
        if ((*ptr_i).value >= k_largest && j < nb_instances){
            (res + j)->id = (*ptr_i).id;  
            (res + j)->value = (*ptr_i).value;  
            ++j;
        } else if (j == nb_instances) {
            return 0;
        }
    }
    return 0;
}

// Get the number of occurences of a character - Useful when parsing
uint32_t nb_occurences(char* s, char c, char* last_element) {
    uint32_t res = 0;
    char *pos_pointer = strchr(s, '\n' );
    while ( pos_pointer < last_element && pos_pointer != NULL) {
        res++;
        s = pos_pointer + 1;
        pos_pointer = strchr(s, '\n' );
    }
    return res;
}

// Parse the mapped region and store the lines into data pairs 
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
        if (id_value == NULL) {
            perror(FILE_BAD_FORMAT_ERROR);
        }
        id = (char*)(malloc(id_value - s + 1));
        memcpy(id, s, id_value - s + 1);

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
