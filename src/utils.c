#include <stdio.h>
#include <string.h>
#include <time.h>

int is_uuid(const char *input) {
	unsigned int a, b, c, d, e;
	int result = sscanf(input, "%08x-%04x-%04x-%04x-%012x", &a, &b, &c, &d, &e);
	return result == 5;
}

const char *trunc_uuid(const char *uuid) {
	char *ret = strdup(uuid);
	ret[8] = '\0';
	return (const char*)ret;
}

time_t localize(time_t utc) {
	const time_t now = time(NULL);
	struct tm now_bd = { 0 };
	localtime_r(&now, &now_bd);

	return utc - now_bd.tm_gmtoff;
}
