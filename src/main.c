#include <stdio.h>
#include <string.h>

#include "command/command.h"

int main(int argc, char *argv[])
{
	options opts;
	parse_options(&opts, argc, argv);

	PGconn *conn = PQconnectdb("dbname=mend");
	if (PQstatus(conn) != CONNECTION_OK) {
		printf("could not connect to database\n");
		return 1;
	}

	char *cmd = opts.identifiers[0];
	if (strcmp(cmd, "le") == 0 || strcmp(cmd, "list-entities") == 0)
		list_entities(conn, &opts);
	return 0;
}
