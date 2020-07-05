#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "../utils.h"

void edit_relation(PGconn *conn, options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		exit(1);
	} else if (!is_uuid(id)) {
		fprintf(stderr, ERR "argument must be a UUID\n");
		exit(1);
	}

	const char *value = options->identifiers[2];
	if (!value) {
		fprintf(stderr, ERR "no value specified\n");
		exit(1);
	}

	const char *const params[] = {
		id,
		value
	};

	PGresult *result = PQexecParams(conn,
			"UPDATE relation "
			"SET note = $2 "
			"WHERE uid = $1 "
			"RETURNING uid",
			2,
			NULL,
			params,
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

	if (PQntuples(result) == 0)
		fprintf(stderr, ERR "note %s not found\n", id);
	else
		printf("%s\n", PQgetvalue(result, 0, 0));
	PQclear(result);
}
