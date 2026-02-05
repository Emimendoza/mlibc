#include <ctype.h>
#include <mlibc/macros.h>

EXPORT
int isspace(const int ch) {
	switch (ch) {
		case ' ':
		case '\f':
		case '\n':
		case '\r':
		case '\t':
		case '\v':
			return 1;
		default:
		return 0;
	}
}
