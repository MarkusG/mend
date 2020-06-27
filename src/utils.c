#include <stdio.h>

int is_uuid(const char *input) {
	unsigned int a, b, c, d, e;
	int result = sscanf(input, "%08x-%04x-%04x-%04x-%012x", &a, &b, &c, &d, &e);
	return result == 5;
}
