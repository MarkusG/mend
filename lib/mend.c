#include "../include/mend.h"
#include "mend.h"

PGconn *_conn;

int mend_init(const char *connection_string) {
	_conn = PQconnectdb(connection_string);
	return (int)PQstatus(_conn);
}

int mend_cleanup() {
	PQfinish(_conn);
	return 0;
}
