#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

#include "headers/file_reader.hpp"

void n_max_val( data_pair *pairs, data_pair new_val, uint32_t nb_instances ) {
   data_pair* res;
   res = pairs;
   for (data_pair* ptr = pairs; ptr < pairs + nb_instances; ptr++){ // complexity in time -> linear
      if ( (*ptr).value < (*res).value ) {
         res = ptr;
      }
   }

   if ( (*res).value < new_val.value ) {
      (*res).value = new_val.value;
      (*res).id = new_val.id;
   }

}

int file_filter(int nb_instances, std::string path, data_pair* res) {
   std::string line;
   uint32_t id, value, pos;
   data_pair data;

   std::ifstream infile; 
   infile.open( path ); 
   
   if ( infile.is_open() ) {  
      while ( std::getline(infile, line) ){ // reading line by line is more scalable
         try {
         pos = line.find( ' ' );
         data.id = std::stoi( line.substr( 0, pos ) );
         data.value = std::stoi( line.substr( pos) );
         n_max_val( res, data, nb_instances ); 
         }
         catch ( std::invalid_argument const &e ) {
            std::cout << "Bad input" << '\n';
         }
         catch ( std::out_of_range const &e ) {
            std::cout << "Integer overflow" << '\n';
         }
      } // -> complexity in time n log(n)

   
   infile.close();
   }
   
   return 0;
}



int main () {  
   int nb_instances = 3;
   std::string path = "sample_file.dat";
   data_pair res[nb_instances] ;
   file_filter( nb_instances, path, res ); 
  
   for ( int i = 0; i < nb_instances ; i++) {
      std::cout << "Result n*" << i << ' ' << res[i].id << '\n';
   }
   
   return 0;
}