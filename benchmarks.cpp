#include <string.h>
#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include <sys/sysinfo.h>
#include<sys/time.h>

#include "headers/file_reader.hpp"
#include "headers/memory_mapping_reader.hpp"


int main (int argc, char *argv[]) {  
    DIR *dir;
    struct dirent *dir_entry;
    char testfiles_dir[13] = "./testfiles/"; 
    struct timeval startTime, endTime;
    long totalTime;
    dir = opendir(testfiles_dir);
    if (dir != NULL){
        dir_entry = readdir(dir);
        while (dir_entry != NULL){
            int nb_instances = 300;
            char* file_path = (char*)malloc(strlen(testfiles_dir) + strlen(dir_entry->d_name)); 
            strcpy(file_path, testfiles_dir);
            strcat(file_path, dir_entry->d_name);
            
            if ( dir_entry->d_type == DT_REG ) {
                std::cout << "Filename path: " << file_path << std::endl; 
                data_pair* res_mapping = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
                data_pair* res_stream = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
                data_pair* res_mapping_parallel = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));


                clock_t begin = clock();
                std::string file(file_path);
                file_stream_filter(file, nb_instances, res_stream);
                clock_t end = clock();
                double time = (double) (end - begin) / CLOCKS_PER_SEC;
                std::cout << "Time Spent to stream version : " << time << std::endl; 


                begin = clock();
                file_mapping_filter(file_path, nb_instances, res_mapping);
                end = clock();
                time = (double) (end - begin) / CLOCKS_PER_SEC;
                std::cout << "Time Spent to mapped version : " << time << std::endl; 

                gettimeofday(&startTime, NULL);
                file_mapping_filter_parallel(file_path, nb_instances, res_mapping_parallel);
                gettimeofday(&endTime, NULL);
                totalTime =  (endTime.tv_sec - startTime.tv_sec) * 1e6L;
                totalTime += (endTime.tv_usec - startTime.tv_usec);

                std::cout << "Time Spent to mapped parallel version : " << (totalTime / 1e6L) << std::endl; 
                
                free(res_mapping);
                free(res_stream);
                free(res_mapping_parallel);
            }
            dir_entry = readdir(dir);
        }
    }  else {
        std::cout << "Test Directory is missing. Create a test directory with name testfiles, and generate testfiles." << std::endl; 
    }
    closedir(dir);

    return 0;
}