#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-non-copyable-objects"
#include <stdio.h>
#include <stdlib.h>
#include <internals/file.h>
#include <errno.h>
#include <stdatomic.h>
#include <alloca.h>

#define SMALL_BUFFER_SIZE 128

volatile int vector_lock = 0;
volatile vector(FILE*) files;

void lock_vector(){
    while(atomic_exchange(&vector_lock, 1) != 0);
}
void unlock_vector(){
    atomic_store(&vector_lock, 0);
}

void lock_file(FILE* file){
    while(atomic_exchange(&file->_lock, 1) != 0);
}
void unlock_file(FILE* file){
    atomic_store(&file->_lock, 0);
}

FILE* stdin = NULL;
FILE* stdout = NULL;
FILE* stderr = NULL;

// Call this for closing stdin, stdout and stderr
void _close_special(FILE* file){
    free(file->_buf);
}

void cleanup_files(){
    fflush(NULL); // Flush all files
    lock_vector();
    _close_special(stdin);
    _close_special(stdout);
    _close_special(stderr);
    while(!vector_empty(files)){
        FILE* file = vector_top(files);
        vector_pop(files);
        if (file->_fd < 3){
            continue;
        }
        free(file->_buf);
        free(file);
    }
    free(stdin); // stdin, stdout and stderr are malloced in the same block
    vector_free(files);
}


int __init_files(){
    if (vector_init(files) != 0){
        return 1;
    }
    FILE* standard_files = malloc(sizeof(FILE)*3);
    if (standard_files == NULL){
        vector_free(files);
        return 1;
    }
    lock_vector();
    standard_files[0]._fd = 0;
    standard_files[0]._mode = _MLIBC_RONLY;
    standard_files[0]._buf = malloc(BUFSIZ* sizeof(uint8_t));
    if (standard_files[0]._buf == NULL){
        free(standard_files);
        vector_free(files);
        return 1;
    }
    standard_files[0]._buf_size = BUFSIZ;
    standard_files[0]._offset._cnt = 0;
    standard_files[0]._buf_begin = 0;
    standard_files[0]._buf_mode = _IOLBF;
    standard_files[0]._buf_cnt = 0;
    standard_files[0]._lock = 0;
    standard_files[0]._vec_index = 0;

    standard_files[1]._fd = 1;
    standard_files[1]._mode = _MLIBC_WONLY;
    standard_files[1]._buf = malloc(BUFSIZ* sizeof(uint8_t));
    standard_files[1]._buf_size = BUFSIZ;
    standard_files[1]._offset._cnt = 0;
    standard_files[1]._buf_begin = 0;
    standard_files[1]._buf_mode = _IOLBF;
    standard_files[1]._buf_cnt = 0;
    standard_files[1]._lock = 0;
    standard_files[1]._vec_index = 1;

    standard_files[2]._fd = 2;
    standard_files[2]._mode = _MLIBC_WONLY;
    standard_files[2]._buf = NULL;
    standard_files[2]._buf_size = 0;
    standard_files[2]._offset._cnt = 0;
    standard_files[2]._buf_begin = 0;
    standard_files[2]._buf_mode = _IONBF;
    standard_files[2]._buf_cnt = 0;
    standard_files[2]._lock = 0;
    standard_files[2]._vec_index = 2;

    // Check that all buffers are valid
    for(int i = 0; i < 2; i++){
        if (standard_files[i]._buf == NULL){
            for(int j = 0; j < i; j++){
                free(standard_files[j]._buf);
            }
            free(standard_files);
            vector_free(files);
            return 1;
        }
    }

    // Set the standard files

    stdin = &standard_files[0];
    stdout = &standard_files[1];
    stderr = &standard_files[2];

    // Add the standard files to the vector

    for (int i = 0; i < 3; i++){
        if (vector_push(files, &standard_files[i]) != 0){
            for(int j = 0; j < i; j++){
                free(standard_files[j]._buf);
            }
            free(standard_files);
            vector_free(files);
            return 1;
        }
    }
    unlock_vector();

    return atexit(&cleanup_files);
}

FILE* fopen(const char* filename, const char* mode){
    flags_t flags = 0;
    uint8_t f_mode = 0;
    if(mode[0] == 'r'){
        flags |= O_RDONLY;
        f_mode = _MLIBC_RONLY;
    }else if(mode[0] == 'w'){
        flags |= O_WRONLY | O_CREAT | O_TRUNC;
        f_mode = _MLIBC_WONLY;
    } else if(mode[0] == 'a'){
        flags |= O_RDWR | O_CREAT | O_APPEND;
        f_mode = _MLIBC_WR;
    } else {
        return NULL;
    }
    if(mode[1] == 'b'){
        // Do nothing
    } else if(mode[1] == '+'){
        // set the first byte to 0 and the second byte to 1
        flags &= 0xFFFE;
        flags |= O_RDWR;
        f_mode = _MLIBC_WR;
    }
    int fd = __open(filename, flags, 0);
    if(fd < 0){
        errno = -fd;
        return NULL;
    }
    FILE* file = malloc(sizeof(FILE));
    if (file == NULL){
        __close(fd);
        errno = ENOMEM;
        return NULL;
    }
    file->_fd = fd;
    file->_mode = f_mode;
    file->_buf = malloc(BUFSIZ* sizeof (char));
    if (file->_buf == NULL){
        errno = ENOMEM;
        __close(fd);
        free(file);
        return NULL;
    }
    file->_buf_size = BUFSIZ;
    _mlibc_off_t offset = __lseek(fd, 0, SEEK_CUR);
    if (offset < 0){
        errno = -offset;
        __close(fd);
        free(file->_buf);
        free(file);
        return NULL;
    }
    file->_offset._cnt = offset;
    file->_buf_begin = offset;
    file->_buf_mode = _IOFBF;
    file->_buf_cnt = 0;
    file->_lock = 0;
    lock_vector();
    file->_vec_index = vector_size(files);
    if (vector_push(files, file) != 0){
        unlock_vector();
        __close(fd);
        free(file->_buf);
        free(file);
        errno = ENOMEM;
        return NULL;
    }
    unlock_vector();
    return file;
}

int fclose(FILE* stream){
    if (stream == NULL) {
        return EOF;
    }
    if (stream->_buf_mode == _IOLBF || stream->_buf_mode == _IOFBF){
        fflush(stream);
    }
    lock_file(stream);
    int ret = __close(stream->_fd);
    if (ret < 0) {
        unlock_file(stream);
        return EOF;
    }
    lock_vector();
    for(size_t i = stream->_vec_index+1; i < vector_size(files); i++){
        vector_get(files, i)->_vec_index--;
    }
    vector_remove_at(files, stream->_vec_index);
    unlock_vector();
    free(stream->_buf);
    free(stream);
    return 0;
}

int fflush_all(){
    lock_vector();
    for(size_t i = 0; i < vector_size(files); i++){
        FILE* file = vector_get(files, i);
        if (file->_buf_mode == _IOLBF || file->_buf_mode == _IOFBF){
            fflush(file);
        }
    }
    unlock_vector();
    return 0;
}

int _mlibc_flush_buffer(int fd, uint8_t* buffer, size_t* buffer_cnt, size_t* file_offset){
    if (*buffer_cnt > 0){
        ssize_t written = __write(fd, buffer, *buffer_cnt);
        if (written < 0){
            return EOF;
        }
        *file_offset += written;
        if ((size_t)written != *buffer_cnt){
            return EOF;
        }
        *buffer_cnt = 0;
    }
    return 0;
}

int fflush(FILE* stream){
    if (stream == NULL){
        return fflush_all();
    }
    if (stream->_buf_mode == _IONBF){
        return 0;
    }
    if (stream->_mode == _MLIBC_RONLY){
        return 0;
    }
    lock_file(stream);
    int ret = _mlibc_flush_buffer(stream->_fd, stream->_buf, &stream->_buf_cnt, &stream->_buf_begin);
    unlock_file(stream);
    return ret;
}

int vfprintf(FILE* stream, const char* format, va_list arg){
    if (stream == NULL || stream->_mode == _MLIBC_RONLY){
        return EOF;
    }
    lock_file(stream);
    uint8_t* buffer_ptr = NULL;
    size_t* buffer_size_ptr = NULL;
    size_t* buffer_cnt_ptr = NULL;
    size_t* file_offset_ptr = &stream->_buf_begin;

    if (stream->_buf_mode == _IONBF){
        void* buffer_and_more = alloca(SMALL_BUFFER_SIZE + sizeof(size_t)*2);
        buffer_ptr = buffer_and_more;
        buffer_size_ptr = (buffer_and_more + SMALL_BUFFER_SIZE);
        buffer_cnt_ptr = (buffer_and_more + SMALL_BUFFER_SIZE + sizeof(size_t));
        *buffer_size_ptr = SMALL_BUFFER_SIZE;
        *buffer_cnt_ptr = 0;
    } else {
        buffer_ptr = stream->_buf;
        buffer_size_ptr = &stream->_buf_size;
        buffer_cnt_ptr = &stream->_buf_cnt;
    }
    int ret = 0;

    for (size_t i = 0 ; format[i] != '\0' ; i++){
        if (*buffer_cnt_ptr == *buffer_size_ptr){
        }
        if (format[i] == '%'){
           i++;
            switch (format[i]) {
                case '%':
                    buffer_ptr[*buffer_cnt_ptr] = '%';
                    (*buffer_cnt_ptr)++;
                    continue;
            }
        }
        buffer_ptr[*buffer_cnt_ptr] = format[i];
        (*buffer_cnt_ptr)++;
        if (format[i] == '\n' && stream->_buf_mode == _IOLBF){
            ret = _mlibc_flush_buffer(stream->_fd, buffer_ptr, buffer_cnt_ptr,file_offset_ptr);
            if (ret != 0) {break;}
            continue;
        }
    }

    unlock_file(stream);
    return ret;
}

int fprintf(FILE* stream, const char* format, ...){
    va_list args;
    va_start(args, format);
    int ret = vfprintf(stream, format, args);
    va_end(args);
    return ret;
}
#pragma clang diagnostic pop