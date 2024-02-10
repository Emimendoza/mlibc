#include <threads.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>

int mtx_init(mtx_t *mtx, int type) {
    if(mtx == NULL) {
        return thrd_error;
    }
    if((type & (mtx_plain | mtx_recursive | mtx_timed)) == 0) {
        return thrd_error;
        // Type must be at least one of the types
    }
    // Invalid type
    if(type == ( mtx_timed | mtx_plain)){
        return thrd_error;
    }
    if (type ==(mtx_plain | mtx_recursive | mtx_timed)){
        return thrd_error;
    }

    mtx->_futex = 0;
    mtx->_type = type;
    mtx->_count = 0;
    mtx->_owner = 0;
    mtx->_is_locked = false;
    return thrd_success;
}

int mtx_lock(mtx_t *mtx){
    if(mtx == NULL){
        return thrd_error;
    }
    // We want to lock the mutex struct
    while(atomic_exchange(&mtx->_is_locked, true) != false){
        // Wait for the lock to be released
    }
    int return_value = thrd_error;

    // TODO: Implement the futex lock


    atomic_store(&mtx->_is_locked, false);
    return return_value;
}