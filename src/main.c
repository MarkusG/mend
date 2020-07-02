#include <stdlib.h>
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
	else if (strcmp(cmd, "ce") == 0 || strcmp(cmd, "create-entity") == 0)
		create_entity(conn, &opts);
	else if (strcmp(cmd, "re") == 0 || strcmp(cmd, "remove-entity") == 0)
		remove_entity(conn, &opts);
	else if (strcmp(cmd, "ca") == 0 || strcmp(cmd, "create-alias") == 0)
		create_alias(conn, &opts);
	else if (strcmp(cmd, "ra") == 0 || strcmp(cmd, "remove-alias") == 0)
		remove_alias(conn, &opts);
	else if (strcmp(cmd, "can") == 0 || strcmp(cmd, "create-annotation") == 0)
		create_annotation(conn, &opts);
	else if (strcmp(cmd, "uan") == 0 || strcmp(cmd, "update-annotation") == 0)
		update_annotation(conn, &opts);
	else if (strcmp(cmd, "ran") == 0 || strcmp(cmd, "remove-annotation") == 0)
		remove_annotation(conn, &opts);

	PQfinish(conn);
	free(opts.identifiers);
	return 0;
}
