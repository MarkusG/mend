#include <stdlib.h>

#include "command.h"
#include "../utils.h"

void remove_relation(PGconn *conn, options *options) {
	if (!options->identifiers[1]) {
		fprintf(stderr, ERR "no identifier specified\n");
		exit(1);
	}

	int i = 1;
	const char *id;
	while ((id = options->identifiers[i])) {
		if (!is_uuid(id)) {
			fprintf(stderr, ERR "'%s' is not a UUID\n", id);
			++i;
			continue;
		}

		PGresult *result = PQexecParams(conn,
				"DELETE FROM relation "
				"WHERE uid = $1",
				1,
				NULL,
				&id,
				NULL,
				NULL,
				0);

		switch (PQresultStatus(result)) {
			case PGRES_COMMAND_OK:
				break;
			default:
				// unexpected response
				fprintf(stderr, ERR "%s: %s",
						PQresStatus(PQresultStatus(result)),
						PQresultErrorMessage(result));
				exit(1);
		}

		++i;
		PQclear(result);
	}
}
