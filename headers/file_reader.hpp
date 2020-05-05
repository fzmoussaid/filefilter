#ifndef FILEREADER_H
#define FILEREADER_H
#include <string>
#include <cstdlib>

#include "struct.hpp"

void n_max_val( data_pair *pairs, data_pair new_val, uint32_t nb_instances );

int file_stream_filter(std::string path,int nb_instances, data_pair* res);

#endif