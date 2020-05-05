#include <string>
#include <cstdlib>
#include <iostream>
#include "headers/memory_mapping_reader.hpp"
#include <sys/sysinfo.h>



int main () {  
    int nb_instances = 3;
    std::string path = "testfiles/sample_file_100000000.dat";
    data_pair* res_pair = (data_pair*)calloc(nb_instances, 2 * sizeof(uint64_t));

    clock_t begin = clock();
    filter_section(path.c_str(), nb_instances, res_pair);
    clock_t end = clock();
    double time = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time Spent to parallel algo : " << time << std::endl;   
    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_pair + i)).id << '\n';
    } 
    
    free(res_pair);
    return 0;
}