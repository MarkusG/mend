#include <stdlib.h>

#include "command.h"
#include "../utils.h"

void new_entity(PGconn *conn, options *options) {
	const char *name = options->identifiers[1];
	if (!name) {
		fprintf(stderr, ERR "no name specified\n");
		exit(1);
	}

	PGresult *result = PQexecParams(conn,
			"INSERT INTO entity (name) "
			"VALUES ($1) "
			"RETURNING uid",
			1,
			NULL,
			&name,
			NULL,
			NULL,
			0);

	switch (PQresultStatus(result)) {
		case PGRES_TUPLES_OK:
			break;
		default:
			// unexpected response
			fprintf(stderr, ERR "%s: %s",
					PQresStatus(PQresultStatus(result)),
					PQresultErrorMessage(result));
			exit(1);
	}

	printf("%s\n", PQgetvalue(result, 0, 0));
	PQclear(result);
}
