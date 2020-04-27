#include <string>
#include <cstdlib>
#include <iostream>
#include "headers/memory_mapping_reader.hpp"



int main () {  
    int nb_instances = 3;
    std::string path = "sample_file500.dat";
    uint32_t res[nb_instances] ;
    data_pair* data;
    uint32_t data_size = 0;
    
    std::cout << "Starting....";
    data = memory_mapper( path.c_str(), &data_size); 
    std::cout << "Memory Mapped";
    uint32_t k_largest = selection( data, data_size, nb_instances );
    // std::cout << "K largest Value is : " << k_largest << std::endl;
    get_inf_values( data, data_size, k_largest, res, nb_instances);
    
    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << *(res + i) << '\n';
    }
    if (data != NULL){
        free(data);
    }

   return 0;
}