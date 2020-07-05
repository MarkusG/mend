#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "_mend.h"

PGconn *_conn;
char *_error;

void _set_error(const char *format, ...) {
	va_list list;
	va_start(list, format);
	vsprintf(_error, format, list);
	va_end(list);
}

const char *mend_error() {
	return _error;
}

int mend_init(const char *connection_string) {
	_error = malloc(128 * sizeof(char));
	_conn = PQconnectdb(connection_string);
	_set_error("could not connect to database");
	return PQstatus(_conn) != CONNECTION_OK;
}

int mend_cleanup() {
	PQfinish(_conn);
	free(_error);
	return 0;
}
