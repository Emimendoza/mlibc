#include <unistd.h>
#include <stdlib.h>
int main() {
	void* ptr = malloc(100);
	free(ptr);
	int page_size = getpagesize();
	return 0;
}