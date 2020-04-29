#include <fstream>
#include <iostream>
#include <string>
using namespace std;


void generate_testfile(int nb_samples){
    string data;
    ofstream outfile;
    outfile.open( ("testfiles/sample_file_" + to_string(nb_samples) + ".dat" ));
    
    for ( int i = 0; i < nb_samples ; i++ ) {
        outfile << i << ' ' << std::rand() << endl;   
    }
    
    outfile.close();  
}

int main() {
    string data;
    ofstream outfile;
    // for (int i = 500; i < 1e6; i = i + 1e3){
        generate_testfile(1e8);
    // }

}