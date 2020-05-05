#include <fstream>
#include <iostream>
#include <string>

#include "headers/struct.hpp"


void generate_testfile(int nb_samples){
    std::string data;
    std::ofstream outfile;
    outfile.open( ("testfiles/sample_file_" + std::to_string(nb_samples) + ".dat" ));
    
    for ( int i = 0; i < nb_samples ; i++ ) {
        outfile << i << ' ' << std::rand() << std::endl;   
    }
    
    outfile.close();  
}

int main() {
    std::string data;
    std::ofstream outfile;
    for (int i = 500; i < 1e8; i = i + 1e4){
        generate_testfile(i);
    }

}