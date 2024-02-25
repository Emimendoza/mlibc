#include <internals/file.h>

int main() {
    char* filename = "this_file_does_not_exist";
    int fd = __open(filename, O_RDONLY, 0);
    return fd>=0;
}
