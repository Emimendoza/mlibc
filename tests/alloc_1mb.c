#include <stdlib.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "MemoryLeak"
int main(){
    uint8_t* ptr = (uint8_t*) malloc(1024*1024);
    for(size_t i = 0; i < 1024*1024; i++){
        ptr[i] = i%256;
    }
    for(size_t i = 0; i < 1024*1024; i++){
        if(ptr[i] != i%256){
            free(ptr);
            return 1;
        }
    }
    uint8_t* n_ptr = (uint8_t*) realloc(ptr, 1024*1024*2);
    if (n_ptr == NULL){
        free(ptr);
        free(n_ptr);
        return 1;
    }
    for(size_t i = 0; i < 1024*1024; i++){
        if(n_ptr[i] != i%256){
            free(n_ptr);
            return 1;
        }
    }
    for(size_t i = 1024*1024; i < 1024*1024*2; i++){
        n_ptr[i] = i%256;
    }
    for(size_t i = 0; i < 1024*1024*2; i++){
        if(n_ptr[i] != i%256){
            free(n_ptr);
            return 1;
        }
    }
    free(n_ptr);
    return 0;
}

#pragma clang diagnostic pop