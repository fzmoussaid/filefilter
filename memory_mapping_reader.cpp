#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h> 
#include <sys/sysinfo.h>
#include <omp.h>
#include <assert.h> 

#include "headers/memory_mapping_reader.hpp"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void get_data_pairs(char* s, data_pair *data, uint16_t* data_size, uint32_t nb_lines, char* last_element, uint16_t section) {
    
    data_pair *ptr;
    char *pos_pointer, *id_value;
    char *id, *value;
    assert(nb_lines != 0);
    uint32_t i = 1;
    int8_t nb_proc = get_nprocs();
    uint32_t nb_total = 0;
    for(int i = 0; i < nb_proc ; i++) 
        *(data_size + i) = 0;
    ptr = data;
    if (section != 0) {
        pos_pointer = strchr(s, '\n' );
        s = pos_pointer + 1;
    }
    pos_pointer = strchr(s, '\n' );


    while( pos_pointer < last_element && pos_pointer != NULL) {
        if ( nb_total >  i * nb_lines / nb_proc) {
            i++;
        }
        id_value = strchr(s, ' ' );
        id = (char*)(malloc(id_value - s + 1));
        // id = (char*)realloc(id, id_value - s);
        memcpy(id, s, id_value - s + 1);
        // std::cout << "point 3-2 : " << (int)(pos_pointer - id_value) << " end" << std::endl;
        // std::cout << "data_size : " << *data_size[i - 1] << std::endl;
        if ((int)(pos_pointer - id_value)  < 0) {
            std::cout << "point 3-2 : " << (int)(pos_pointer - id_value) << " end" << std::endl;    
        }
        value = (char*)(malloc(pos_pointer - id_value + 1));
        // value = (char*)realloc(value, pos_pointer - id_value);
        memcpy(value, id_value, pos_pointer - id_value + 1);
        

        // try {
        (*ptr).id = std::strtol( id , NULL, 10);
        (*ptr).value = std::strtol( value , NULL, 10);
        // }
        //  catch ( std::invalid_argument const &e ) {
        //     std::cout << "Bad Parsing" << '\n';
        // if((*ptr).id >= 900060000) {
        // std::cout << "Id " << (*ptr).id << '\n';
                
        // std::cout << "Value " << (*ptr).value << '\n';
        // //     }
        //  }
        //  catch ( std::out_of_range const &e ) {
        //     std::cout << "Integer overflow" << '\n';

        // }

        ++ptr;
        ++(*(data_size + i - 1));
        ++nb_total;
        s = pos_pointer + 1;
        pos_pointer = strchr(s, '\n' );
        free(id);
        free(value);

    }


}

data_pair* section_memory_mapping( int* fd, uint16_t* data_size, uint16_t section) {

    size_t pagesize = getpagesize();
    uint16_t nb_lines = 0;
    char tmp = ' ';
    uint8_t nb_proc = get_nprocs();
    data_pair* data;    

    off64_t file_size = lseek(*fd, 0, SEEK_END);
    // std::cout << "File    Size is aggain  " << file_size << std::endl;
    off_t pos = 0;
    // std::cout << "Mapping of Section : " << section  << " Started.\n"; 
    // omp_set_lock(lock);
    if ((section + 1) * pagesize * nb_proc < file_size) {
    
        while(tmp != '\n') {

            pos++;
            lseek(*fd, (section + 1) * pagesize * nb_proc + pos, SEEK_SET);   
            read(*fd, &tmp, 1);
            // std::cout << "read : " << tmp << std::endl;
            
        }
    } else {
        pos = 0;
    }
    
    // lseek(*fd, 0, SEEK_SET);  
    // std::cout << "pos : ..............................................................." << pos << std::endl;
    char* region = (char*)(mmap(NULL, pagesize * nb_proc + pos, PROT_READ, MAP_PRIVATE, *fd, section * pagesize * nb_proc));

    // std::cout << " Maaaapped..............................................................." << section << std::endl;
    if (region == MAP_FAILED) {
       perror("Could not mmap");
       return NULL;
    }
    // std::cout << "Mapping of Section " << section  << " Ended.\n";  
    nb_lines = nb_occurences( region, '\n' , region + pagesize * nb_proc + pos);
    // std::cout << nb_lines << '\n';  
    data = (data_pair*) malloc( nb_lines * 2 * sizeof(uint64_t)); // allocating memory at once is more performant
    // std::cout << "Dataallocend in sec " << section << '\n';  
    get_data_pairs(region, data, data_size, nb_lines, region + pagesize * nb_proc + pos, section);
    
    munmap(region, pagesize * nb_proc + pos);
    lseek(*fd, 0, SEEK_SET); 
    
    return data;
  
}


void filter_section(const char* path, uint16_t nb_instances, data_pair* res_pair ) {
    int fd = open(path, O_RDONLY);
    int8_t nb_proc = get_nprocs();
    size_t pagesize = getpagesize();
    uint64_t file_size = lseek(fd, 0, SEEK_END);
    std::cout << "File Size is " << file_size << std::endl;
    uint16_t* data_size = (uint16_t*)calloc(nb_proc, sizeof(uint16_t));
    data_pair* data;
    uint64_t Nb_sections = (file_size % (nb_proc * pagesize) == 0)? file_size / (nb_proc * pagesize) : file_size / (nb_proc * pagesize) + 1;;
    std::cout << "Nb sections :" << Nb_sections << '\n'; 
    uint16_t offset = 0;
    // #pragma omp parallel for
    for (uint16_t section = 0; section < Nb_sections; section++) {
        data = section_memory_mapping( &fd, data_size, section);   
        // std::cout << "data_size : " << *data_size << std::endl;
        
        for (uint32_t i =  0; i < nb_proc; i++){
                offset = 0;
                if(i != 0)
                    for (uint32_t j =  0; j < i; j++)
                        offset = offset + data_size[j];
                n_max_val_section( res_pair, nb_instances, data + offset, *(data_size + i) ); 

            }
        if (data != NULL){
            free(data);
        }
    }
    free(data_size);
    std::cout << "Nb sections :" << Nb_sections << '\n'; 

        // // }
    close(fd);


}




void n_max_val_section( data_pair *res, uint16_t nb_instances, data_pair *pairs ,  uint16_t data_size) {  
    for (data_pair* ptr = pairs; ptr < pairs + data_size; ptr++){ 
        // std::cout << "data : " << (*(ptr)).id  << std::endl;
        n_max_val( res, ptr, nb_instances );
    }

}

void n_max_val( data_pair *pairs, data_pair* new_val, uint16_t nb_instances ) {  
   data_pair* res;
   res = pairs; 
//    std::cout << "Id comp :" << (*new_val).id << " - "; 
   for (data_pair* ptr = pairs; ptr < pairs + nb_instances; ptr++){ 
      if ( (*ptr).value < (*res).value ) {
         res = ptr;
      }
   }

   if ( (*res).value < (*new_val).value ) {
      (*res).value = (*new_val).value;
      (*res).id = (*new_val).id;
   }

}

// look-up the k-th largest value in an array using the selection algorithm -> complexity on time : O(n^2) worst case scenario
uint32_t selection( data_pair *data, uint32_t data_size, uint32_t k ) { 
    uint32_t maxValue;
    data_pair* tmp_pair = (data_pair*)malloc( 2 * sizeof(uint32_t));
    for (data_pair* ptr_i = data; ptr_i < data + k ; ptr_i++){
        maxValue = (*ptr_i).value;
        for (data_pair* ptr_j = ptr_i + 1; ptr_j < data + data_size ; ptr_j++){
            if ( (*ptr_j).value >= maxValue ) {
                maxValue = (*ptr_j).value;
                *tmp_pair = *ptr_i;
                *ptr_i = *ptr_j;
                *ptr_j = *tmp_pair;
            }
        }
    }
    free(tmp_pair);

    return (*(data + k)).value;
}

uint32_t get_inf_values( data_pair* data, uint16_t data_size,  uint32_t k_largest, uint32_t* res, uint16_t nb_instances) {
    uint32_t j = 0;
    for (data_pair* ptr_i = data; ptr_i < data + data_size ; ptr_i++){
        if ((*ptr_i).value >= k_largest && j < nb_instances){
            *(res + j) = (*ptr_i).id;  
            ++j;
        } else if (j == nb_instances) {
            return 0;
        }
    }
    return 0;
}

uint32_t nb_occurences(char* s, char c, char* last_element) { 
    uint32_t res = 0; 
    // std::cout << "Nb occ start " << std::endl;  
    char *ptr = s, *pos_pointer;
    // for (char* i = s; i <  last_element; i++) {
         
    //     if (i == NULL) {
    //         std::cout << "NULL : " << std::endl; 
    //         return res; 
    //     }
    //     if (*(i) == c) {
    //         res++;
    //     }
    // }   
    pos_pointer = strchr(s, '\n' );
    while ( pos_pointer < last_element && pos_pointer != NULL) {
        res++;    
        s = pos_pointer + 1;
        pos_pointer = strchr(s, '\n' );
        // std::cout << "Res " << res << std::endl;    
    }
    // std::cout << "Nb occ end " << std::endl;        
  
    return res; 
} 
