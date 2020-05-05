#include <string.h>
#include <cstdlib>
#include <iostream>
#include <dirent.h>

#include "headers/file_reader.hpp"
#include "headers/memory_mapping_reader.hpp"

void sort_by_index( data_pair *data, uint32_t data_size ) { 
    uint32_t maxIndex;
    data_pair* tmp_pair = (data_pair*)malloc( 2 * sizeof(uint32_t));
    for (data_pair* ptr_i = data; ptr_i < data + data_size - 1 ; ptr_i++){
        maxIndex = (*ptr_i).value;
        for (data_pair* ptr_j = ptr_i + 1; ptr_j < data + data_size ; ptr_j++){
            if ( (*ptr_j).id >= maxIndex ) {
                maxIndex = (*ptr_j).value;
                *tmp_pair = *ptr_i;
                *ptr_i = *ptr_j;
                *ptr_j = *tmp_pair;
            }
        }
    }
    free(tmp_pair);

}

bool check_equal_datapairs(data_pair* array1, data_pair* array2, uint32_t nb_instances) {
    sort_by_index(array1, nb_instances);
    sort_by_index(array2, nb_instances);
    for (uint32_t i = 0; i < nb_instances ; i++) {
        if (  (*(array1 + i)).value != (*(array2 + i)).value  ) {
            return false;
        }
    }
    return true;
}


int main (int argc, char *argv[]) {  
    DIR *dir;
    struct dirent *dir_entry;
    char testfiles_dir[13] = "./testfiles/"; 
    dir = opendir(testfiles_dir);
    if (dir != NULL){
        while (dir_entry = readdir(dir)){
            int nb_instances = 3;
            char* file_path = (char*)malloc(strlen(testfiles_dir) + strlen(dir_entry->d_name)); 
            strcpy(file_path, testfiles_dir);
            strcat(file_path, dir_entry->d_name);
            
            if ( dir_entry->d_type == DT_REG ) {
                std::cout << "Filename path: " << file_path << std::endl; 
                data_pair* res_mapping = (data_pair*)calloc(nb_instances, 2 * sizeof(uint64_t));
                data_pair* res_io = (data_pair*)calloc(nb_instances, 2 * sizeof(uint64_t));

                clock_t begin = clock();
                filter_section(file_path, nb_instances, res_mapping);
                clock_t end = clock();
                double time = (double) (end - begin) / CLOCKS_PER_SEC;
                std::cout << "Time Spent to mapped version : " << time << std::endl; 

                begin = clock();
                std::string file(file_path);
                file_filter(file, nb_instances, res_io);
                end = clock();
                time = (double) (end - begin) / CLOCKS_PER_SEC;
                std::cout << "Time Spent to io version : " << time << std::endl; 
                
                for ( int i = 0; i < nb_instances ; i++) {
                    std::cout << "Result n*" << i << ' ' << (*(res_mapping + i)).id << '\n';
                } 
                for ( int i = 0; i < nb_instances ; i++) {
                    std::cout << "Result n*" << i << ' ' << (*(res_io + i)).id << '\n';
                } 
                // compare result between the two versions

                if (check_equal_datapairs(res_io, res_mapping, nb_instances)) {
                    std::cout << "Same Results for the file : " << dir_entry->d_name << std::endl; 
                } else {
                    std::cout << "Different Results for the file : " << dir_entry->d_name << std::endl; 
                }
                free(res_mapping);
                free(res_io);
            }
        }
    }
    closedir(dir);

    return 0;
}