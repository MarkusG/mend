#include <stdlib.h>

#include "command.h"
#include "../utils.h"

void remove_alias(PGconn *conn, options *options) {
	if (!options->identifiers[1]) {
		fprintf(stderr, ERR "no identifier specified\n");
		exit(1);
	}

	int i = 1;
	const char *id;
	while ((id = options->identifiers[i])) {
		const char *query;
		if (is_uuid(id)) {
			query =
				"DELETE FROM alias "
				"WHERE uid = $1 "
				"RETURNING entity";
		} else {
			query =
				"DELETE FROM alias "
				"WHERE value = $1 "
				"RETURNING entity";
		}

		PGresult *result = PQexecParams(conn,
				query,
				1,
				NULL,
				&id,
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

		if (PQntuples(result) == 0) {
			fprintf(stderr, WARN "no alias %s\n", id);
			++i;
			continue;
		}

		++i;
		PQclear(result);
	}
}
