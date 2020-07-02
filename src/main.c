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
	else if (strcmp(cmd, "ne") == 0 || strcmp(cmd, "new-entity") == 0)
		new_entity(conn, &opts);
	else if (strcmp(cmd, "re") == 0 || strcmp(cmd, "rm-entity") == 0)
		remove_entity(conn, &opts);
	else if (strcmp(cmd, "na") == 0 || strcmp(cmd, "new-alias") == 0)
		new_alias(conn, &opts);
	else if (strcmp(cmd, "ra") == 0 || strcmp(cmd, "rm-alias") == 0)
		remove_alias(conn, &opts);
	else if (strcmp(cmd, "nan") == 0 || strcmp(cmd, "new-note") == 0)
		new_note(conn, &opts);
	else if (strcmp(cmd, "ean") == 0 || strcmp(cmd, "edit-note") == 0)
		edit_note(conn, &opts);
	else if (strcmp(cmd, "ran") == 0 || strcmp(cmd, "rm-note") == 0)
		remove_note(conn, &opts);
	else if (strcmp(cmd, "nr") == 0 || strcmp(cmd, "new-relation") == 0)
		new_relation(conn, &opts);
	else if (strcmp(cmd, "er") == 0 || strcmp(cmd, "edit-relation") == 0)
		edit_relation(conn, &opts);
	else if (strcmp(cmd, "rr") == 0 || strcmp(cmd, "rm-relation") == 0)
		remove_relation(conn, &opts);

	PQfinish(conn);
	free(opts.identifiers);
	return 0;
}
