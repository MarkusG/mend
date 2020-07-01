#include <stdlib.h>

#include "command.h"
#include "../utils.h"

void remove_entity(PGconn *conn, options *options) {
	if (!options->identifiers[1]) {
		fprintf(stderr, "No identifier specified\n");
		exit(1);
	}

	int i = 1;
	const char *id;
	while ((id = options->identifiers[i])) {
		if (!is_uuid(id)) {
			fprintf(stderr, "Identifier '%s' is not a UUID\n", id);
			++i;
			continue;
		}

		PGresult *result = PQexecParams(conn,
				"DELETE FROM entity "
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
				fprintf(stderr, "%s: %s",
						PQresStatus(PQresultStatus(result)),
						PQresultErrorMessage(result));
				exit(1);
		}
		++i;
		PQclear(result);
	}
}
