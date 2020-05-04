# include <omp.h>
#include "struct.hpp"

uint32_t get_inf_values( data_pair* data, uint16_t data_size,  uint32_t k_largest, uint32_t* res, uint16_t nb_instances);

uint32_t selection( data_pair *data, uint16_t data_size, uint32_t k );

uint32_t nb_occurences(char* s, char c, char* last_element);

data_pair* section_memory_mapping( int* fd, uint32_t* data_size, uint16_t section);

void n_max_val( data_pair *pairs, data_pair* new_val, uint16_t nb_instances );

void n_max_val_section( data_pair *res, uint16_t nb_instances, data_pair *pairs ,  uint16_t data_size);

void filter_section(const char* path, uint16_t nb_instances, data_pair* res_pair );

void get_data_pairs(char* s, data_pair *data, uint16_t *data_size, uint32_t nb_lines, char* last_element) ;

data_pair* memory_mapper(const char* path, uint16_t* data_size);