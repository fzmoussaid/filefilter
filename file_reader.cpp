#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <sys/sysinfo.h>

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

int file_stream_filter(std::string path, int nb_instances, data_pair* res) {
   std::string line;
   uint32_t pos;
   data_pair data;

   std::ifstream infile; 
   infile.open( path ); 
   
   for ( data_pair* ptr = res; ptr < res + nb_instances ; ptr++) {
      ptr->id = 0;
      ptr->value = 0;
   } 
   
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
      } // -> complexity in time n log(nb_instances) (n is the number of lines and nb_instances the number of largest elements required to extract)

   
   infile.close();
   }
   
   return 0;
}