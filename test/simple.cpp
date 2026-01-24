#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>

static const int page_size =  [] {
	return getpagesize();
}();
thread_local int32_t test = 42;

int main() {

	void* ptr = malloc(test + page_size);
	test += 1;
	free(ptr);
	ptr = malloc(test + page_size);
	free(ptr);
	return 0;
}
