struct data_pair {
   uint32_t id;
   uint32_t value;
};

uint32_t get_inf_values( data_pair* data, uint32_t data_size,  uint32_t k_largest, uint32_t* res, uint32_t nb_instances);

uint32_t selection( data_pair *data, uint32_t data_size, uint32_t k );

uint32_t nb_occurences(char* s, char c, uint32_t nb);

data_pair* memory_mapper(const char* path, uint32_t* data_size);

void n_max_val( data_pair *pairs, data_pair new_val, uint32_t nb_instances );

data_pair* filter_section(const char* path, uint32_t nb_instances, data_pair* res_pair  );