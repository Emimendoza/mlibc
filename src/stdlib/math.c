#include <math.h>

// This should be compiled with O2 or higher

int abs(int n){
    if(n < 0){
        return -n;
    }
    return n;
}

long int labs(long int n){
    if(n < 0){
        return -n;
    }
    return n;
}

long long int llabs(long long int n){
    if(n < 0){
        return -n;
    }
    return n;
}

// This uses two div instructions on O0 but only one on O2
div_t div(int x, int y){
    div_t result;
    result.quot = x/y;
    result.rem = x%y;
    return result;
}