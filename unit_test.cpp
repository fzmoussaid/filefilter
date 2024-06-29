#include <cstdlib>
#include "headers/file_reader.hpp"

//Utils

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

// Correctness tests
uint32_t simple_test() {
    std::string file ="tests/simple_test.txt";
    uint32_t X = 5;

    data_pair* res_test_one = (data_pair*)calloc(nb_instances, 2 * sizeof(uint32_t));

    file_stream_filter(file, nb_instances, res_stream);

    if (check_equal_datapairs(res_stream, res_test_one, X)) {
        std::cout << "Correct Results for stream version. " << std::endl; 
    } else {
        std::cout << "Wrong Results for stream version." << std::endl; 
    }

}

int main(int argc, char *argv[]) {
    simple_test();
}