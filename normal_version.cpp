#include <string>
#include <cstdlib>
#include <iostream>
#include "limits.h"

#include "headers/file_reader.hpp"
#define MAX UINT_MAX

const char* ERROR_MSG_OPEN_FILE = "Error opening file"; 

// data_pair* read_from_file( uint32_t nb_instances, const char * path, data_pair* res ) {
//    std::string str;  
//    std::istream&  flag = getline(std::cin, str);
      
//    std::cout << "The entereed string is " << str << std::endl;
//    FILE *fp;
   

//    fp = fopen(path , "r");
//    if( fp == NULL ) {
//       perror(ERROR_MSG_OPEN_FILE);
//       return res;
//    }

//    if( fgets (str, UINT_MAX, fp)!=NULL ) {
//       /* writing content to stdout */
//       puts(str);
//    }
//    fclose(fp);
   
//    return res;
// }
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

int main() {
   std::string line;
   uint32_t id, value, pos;
   data_pair data;
   uint32_t nb_instances = 3;

   data_pair* res = (data_pair*)calloc(nb_instances, 2 * sizeof(uint64_t));
   clock_t begin = clock();
   while( !std::cin.eof() ) {
      getline(std::cin, line);
      // std::cout << "The entereed string is " << line << std::endl;  
      try {
         pos = line.find( ' ' );
         data.id = std::stol( line.substr( 0, pos ) );
         data.value = std::stol( line.substr( pos) );
         n_max_val( res, data, nb_instances ); 
         }
         catch ( std::invalid_argument const &e ) {
            std::cout << "Bad input" << '\n';
         }
         catch ( std::out_of_range const &e ) {
            std::cout << "Integer overflow" << '\n';
         }
   }
       clock_t end = clock();
    double time = (double) (end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time Spent to parallel algo : " << time << std::endl;   

   for ( int i = 0; i < nb_instances ; i++) {
      std::cout << "Result n*" << i << ' ' << res[i].id << '\n';
   }

}