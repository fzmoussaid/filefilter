#include <string>
#include <cstdlib>
#include <iostream>
#include "headers/memory_mapping_reader.hpp"
#include "headers/file_reader.hpp"
#include <sys/sysinfo.h>



int main (int argc, char *argv[]) {  
    int nb_instances = 3;
    std::string path;
    
    if (argc < 2)
        path = "";
    else 
        path = argv[1];

    data_pair* res_pair = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));

    clock_t begin = clock();
    file_mapping_filter_section(path.c_str(), nb_instances, res_pair);
    clock_t end = clock();
    double time = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time Spent : " << time << std::endl;   
    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_pair + i)).id << '\n';
    } 

    free(res_pair);
    res_pair = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
    begin = clock();
    file_mapping_filter(path.c_str(), nb_instances, res_pair);
    end = clock();
    time = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time Spent : " << time << std::endl;   
    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_pair + i)).id << '\n';
    } 
    
    free(res_pair);
    // res_pair = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
    
    // begin = clock();
    // file_stream_filter(path, nb_instances, res_pair);
    // end = clock();
    // time = (double) (end - begin) / CLOCKS_PER_SEC;
    // std::cout << "Time Spent : " << time << std::endl;   
    // for ( int i = 0; i < nb_instances ; i++) {
    //     std::cout << "Result n*" << i << ' ' << (*(res_pair + i)).id << '\n';
    // } 
    // free(res_pair);
    return 0;
}