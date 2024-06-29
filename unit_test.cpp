#include <cstdlib>
#include <map>
#include "headers/file_reader.hpp"

using namespace std;
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

bool compare_maps(map<string, int> map1, map<string, int> map2) {
    return map1.size() == map2.size() && equal( map1.begin(), map1.end(), map2.begin() )
}

// Correctness tests
int simple_test() {
    string file ="tests/simple_test.txt";
    uint32_t X = 5;

    map<string, int> res_test_one = x_largest_values(file, X);
    map<string, int> expected_res;

    expected_res["1789"] = 290;
    expected_res["1862"] = 2972;
    expected_res["17801"] = 156;
    expected_res["982"] = 78;
    expected_res["728261"] = 78;

    if (compare_maps(expected_res, res_test_one)) {
        std::cout << "Correct Results for stream version. " << std::endl; 
    } else {
        std::cout << "Wrong Results for stream version." << std::endl; 
    }
    return 0;
}

int main(int argc, char *argv[]) {
    simple_test();
    return 0;
}