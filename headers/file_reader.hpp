#include <string>
#include <cstdlib>

struct data_pair {
   uint32_t id;
   uint32_t value;
};

void n_max_val( data_pair *pairs, data_pair new_val, uint32_t nb_instances );

int file_filter(int nb_instances, std::string path, data_pair* res);