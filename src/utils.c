#include <stdio.h>
#include <time.h>

int is_uuid(const char *input) {
	unsigned int a, b, c, d, e;
	int result = sscanf(input, "%08x-%04x-%04x-%04x-%012x", &a, &b, &c, &d, &e);
	return result == 5;
}

time_t localize(time_t utc) {
	const time_t now = time(NULL);
	struct tm now_bd = { 0 };
	localtime_r(&now, &now_bd);

	return utc - now_bd.tm_gmtoff;
}
