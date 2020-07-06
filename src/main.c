#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#include "../include/mend.h"
#include "command/command.h"
#include "options.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	options opts;
	parse_options(&opts, argc, argv);

	if (mend_init("dbname=mend")) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	PGconn *conn = PQconnectdb("dbname=mend");
	if (PQstatus(conn) != CONNECTION_OK) {
		printf("could not connect to database\n");
		return 1;
	}

	char *cmd = opts.identifiers[0];
	int exit_code;
	if (strcmp(cmd, "le") == 0 || strcmp(cmd, "list-entities") == 0)
		exit_code = list_entities(&opts);
	else if (strcmp(cmd, "ne") == 0 || strcmp(cmd, "new-entity") == 0)
		exit_code = new_entity(&opts);
	else if (strcmp(cmd, "re") == 0 || strcmp(cmd, "rm-entity") == 0)
		exit_code = remove_entity(&opts);
	else if (strcmp(cmd, "na") == 0 || strcmp(cmd, "new-alias") == 0)
		exit_code = new_alias(&opts);
	else if (strcmp(cmd, "ra") == 0 || strcmp(cmd, "rm-alias") == 0)
		exit_code = remove_alias(&opts);
	else if (strcmp(cmd, "nn") == 0 || strcmp(cmd, "new-note") == 0)
		exit_code = new_note(&opts);
	else if (strcmp(cmd, "en") == 0 || strcmp(cmd, "edit-note") == 0)
		exit_code = edit_note(&opts);
	else if (strcmp(cmd, "rn") == 0 || strcmp(cmd, "rm-note") == 0)
		exit_code = remove_note(&opts);
	else if (strcmp(cmd, "nr") == 0 || strcmp(cmd, "new-relation") == 0)
		new_relation(conn, &opts);
	else if (strcmp(cmd, "er") == 0 || strcmp(cmd, "edit-relation") == 0)
		edit_relation(conn, &opts);
	else if (strcmp(cmd, "rr") == 0 || strcmp(cmd, "rm-relation") == 0)
		remove_relation(conn, &opts);

	PQfinish(conn);
	mend_cleanup();
	free(opts.identifiers);
	return exit_code;
}
