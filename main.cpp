#include <string>
#include <cstdlib>
#include <iostream>
#include "headers/memory_mapping_reader.hpp"
#include <sys/sysinfo.h>



int main () {  
    int nb_instances = 30;
    std::string path = "testfiles/sample_file_500.dat";
    // uint32_t res[nb_instances] ;
    data_pair* res_pair = (data_pair*)malloc(nb_instances * 2 * sizeof(uint32_t));
    

    std::cout << "Nb configured processors : " << get_nprocs_conf() << std::endl;
    std::cout << "Nb processors : " << get_nprocs() << std::endl;
    clock_t begin = clock();
    res_pair = filter_section(path.c_str(), nb_instances, res_pair);
    clock_t end = clock();
    double time = (double) (end - begin) / CLOCKS_PER_SEC;
    // std::cout << "Time Spent to parallel algo : " << time << std::endl;    
    

   return 0;
}