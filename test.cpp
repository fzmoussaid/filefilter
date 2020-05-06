#include <string.h>
#include <cstdlib>
#include <iostream>
#include <dirent.h>
#include <fstream>


#include "headers/file_reader.hpp"
#include "headers/memory_mapping_reader.hpp"



// Sort the data pairs by index
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

// Compare datapair arrays
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

// Check if there are duplicates
bool check_equal_datapairs(data_pair* array1, uint32_t nb_instances) {
    sort_by_index(array1, nb_instances);
    for (uint32_t i = 0; i < nb_instances - 1; i++) {
        if (  (*(array1 + i)).id == (*(array1 + i + 1)).id  ) {
            return true;
        }
    }
    return false;
}

void unit_test_one() {
    std::string data;
    std::ofstream outfile;
    outfile.open( ("testfiles/unit_test_one.dat" ));
    
    for ( int i = 0; i < 500 ; i++ ) {
        outfile << i << ' ' << i << std::endl;   
    }
    
    outfile.close();  
}

void unit_test_two() {
    std::string data;
    std::ofstream outfile;
    outfile.open( ("testfiles/unit_test_two.dat" ));
    
    for ( int i = 0; i < 500 ; i++ ) {
        if ((i == 30) || (i == 31) || (i == 32))
            outfile << i << ' ' << i << std::endl;   
        else
            outfile << i << ' ' << 0 << std::endl;   
    }
    
    outfile.close();  
}

int main (int argc, char *argv[]) {  
    unit_test_one();
    unit_test_two();
    int nb_instances = 3;
    char file_path_one[] = "testfiles/unit_test_one.dat" ;

    data_pair* res_mapping = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
    data_pair* res_mapping_parallel = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
    data_pair* res_stream = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));

    data_pair* res_test_one = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));
    data_pair* res_test_two = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));

    for (int i = 0; i < nb_instances; i++) {
        (res_test_one + i)->id = 497 + i;
        (res_test_one + i)->value = 497 + i;
    }

    file_mapping_filter(file_path_one, nb_instances, res_mapping);
    file_mapping_filter_parallel(file_path_one, nb_instances, res_mapping_parallel);
    std::string file(file_path_one);
    file_stream_filter(file, nb_instances, res_stream);


    if (check_equal_datapairs(res_test_one, res_mapping, nb_instances)) {
        std::cout << "Correct Results for mapping version." << std::endl; 
    } else {
        std::cout << "Wrong Results for mapping version." << std::endl; 
    }

    if (check_equal_datapairs(res_test_one, res_mapping_parallel, nb_instances)) {
        std::cout << "Correct Results for mapping parallel version." << std::endl; 
    } else {
        std::cout << "Wrong Results for mapping parallel version." << std::endl; 
    }

    if (check_equal_datapairs(res_stream, res_test_one, nb_instances)) {
        std::cout << "Correct Results for stream version. " << std::endl; 
    } else {
        std::cout << "Wrong Results for stream version." << std::endl; 
    }

    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_mapping_parallel + i)).id << '\n';
    } 

    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_mapping + i)).id << '\n';
    } 


    for (int i = 0; i < nb_instances; i++) {
        (res_test_two + i)->id = 30 + i;
        (res_test_two + i)->value = 30 + i;
    }
    char file_path_two[] = "testfiles/unit_test_two.dat" ;
    file_mapping_filter(file_path_two, nb_instances, res_mapping);
    std::string file_two(file_path_two);
    file_stream_filter(file_two, nb_instances, res_stream);
    file_mapping_filter_parallel(file_path_two, nb_instances, res_mapping_parallel);


    if (check_equal_datapairs(res_test_two, res_mapping, nb_instances)) {
        std::cout << "Correct Results for mapping version." << std::endl; 
    } else {
        std::cout << "Wrong Results for mapping version." << std::endl; 
    }

    if (check_equal_datapairs(res_stream, res_test_two, nb_instances)) {
        std::cout << "Correct Results for stream version. " << std::endl; 
    } else {
        std::cout << "Wrong Results for stream version."  << std::endl; 
    }

    if (check_equal_datapairs(res_test_two, res_mapping_parallel, nb_instances)) {
        std::cout << "Correct Results for mapping parallel version." << std::endl; 
    } else {
        std::cout << "Wrong Results for mapping parallel version." << std::endl; 
    }

    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_mapping_parallel + i)).id << '\n';
    } 

    for ( int i = 0; i < nb_instances ; i++) {
        std::cout << "Result n*" << i << ' ' << (*(res_mapping + i)).id << '\n';
    } 

    free(res_mapping);
    free(res_stream);
    free(res_mapping_parallel);
    free(res_test_one);
    free(res_test_two);
   
    return 0;
}