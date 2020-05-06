#include <string>
#include <cstdlib>
#include <iostream>
#include <sys/sysinfo.h>
#include<sys/time.h>

#include "headers/memory_mapping_reader.hpp"
#include "headers/file_reader.hpp"




int main (int argc, char *argv[]) {  
    int nb_instances = 3;
    std::string path;
    struct timeval startTime, endTime;
    long totalTime;
    
    if (argc < 2)
        path = "";
    else 
        path = argv[1];

    data_pair* res_pair = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
    clock_t begin = clock();
    file_mapping_filter(path.c_str(), nb_instances, res_pair);
    clock_t end = clock();
    double time = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time Spent : " << time << std::endl;   
    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_pair + i)).id << '\n';
    } 
    
    free(res_pair);

    res_pair = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
    std::cout << path << std::endl;
    begin = clock();
    gettimeofday(&startTime, NULL);
    file_mapping_filter_parallel(path.c_str(), nb_instances, res_pair);
    gettimeofday(&endTime, NULL);
    totalTime =  (endTime.tv_sec - startTime.tv_sec) * 1e6L;
    totalTime += (endTime.tv_usec - startTime.tv_usec);
    end = clock();
    time = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time CPU Spent in Parallel : " << time << std::endl;   
    std::cout << "Real Time Spent in Parallel : " << (totalTime / 1e6L) << std::endl;   
    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_pair + i)).id << '\n';
    } 

    free(res_pair);

    res_pair = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
    
    begin = clock();
    file_stream_filter(path, nb_instances, res_pair);
    end = clock();
    time = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time Spent in stream : " << time << std::endl;   
    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_pair + i)).id << '\n';
    } 
    free(res_pair);
    return 0;
}