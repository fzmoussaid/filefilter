#include <fstream>
#include <iostream>
#include <string>
using namespace std;


void generate_testfile(int nb_samples){
    string data;
    ofstream outfile;
    int nb_samples = 500;
    outfile.open( ("sample_file" + to_string(nb_samples) + ".dat" ));
    
    for ( int i = 0; i < nb_samples ; i++ ) {
        outfile << i << ' ' << std::rand() << endl;   
    }
    
    outfile.close();  
}

int main() {
    string data;
    ofstream outfile;
    int nb_samples = 500;
    generate_testfile(nb_samples);

}