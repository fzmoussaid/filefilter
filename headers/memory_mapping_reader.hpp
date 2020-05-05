#ifndef MEMMAP_H 
#define MEMMAP_H 
#include <omp.h>
#include "struct.hpp"

const char FILE_BAD_FORMAT_ERROR[] = "File Bad Format.";
const char ID_VALUE_ERROR[] = "Non Numerical ID or Value Found.";
const char OVERFLOW_ERROR[] = "Integer Overflow.";
const char MAPPING_ERROR[] = "Could not mmap";

uint32_t get_inf_values( data_pair* data, uint16_t data_size,  uint32_t k_largest, uint32_t* res, uint32_t nb_instances);

uint32_t selection( data_pair *data, uint16_t data_size, uint32_t k );

uint32_t nb_occurences(char* s, char c, char* last_element);

data_pair* memory_mapping( int* fd, data_pair* res, uint32_t nb_instances);

void n_max_val( data_pair *pairs, data_pair* new_val, uint32_t nb_instances );

void n_max_val_section( data_pair *res, uint32_t nb_instances, data_pair *pairs ,  uint16_t data_size);

void file_mapping_filter(const char* path, uint32_t nb_instances, data_pair* res_pair ) ;

void get_data_pairs(char* s, data_pair *data, uint16_t *data_size, uint32_t nb_lines, char* last_element) ;

void get_k_max( char* region,  uint64_t data_size, data_pair *res, uint32_t nb_instances);

void get_k_largest_section( char* region, uint64_t data_size, data_pair *res, uint32_t k);

void file_mapping_filter_section(const char* path, uint32_t k, data_pair* res_pair );

uint32_t get_inf_values( data_pair* data, uint32_t data_size,  uint32_t k_largest, data_pair* res, uint16_t nb_instances);

uint32_t selection( data_pair *data, uint32_t data_size, uint32_t k );

#endif