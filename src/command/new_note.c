#include <stdlib.h>

#include "command.h"
#include "../utils.h"

// TODO open the user's editor to create the note
void new_note(PGconn *conn, options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		exit(1);
	}

	const char *value = options->identifiers[2];
	if (!value) {
		fprintf(stderr, ERR "no value specified\n");
		exit(1);
	}

	const char *entity = id;
	if (!is_uuid(id)) {
		PGresult *entity_result = PQexecParams(conn,
				"SELECT uid "
				"FROM entity "
				"WHERE name = $1",
				1,
				NULL,
				&id,
				NULL,
				NULL,
				0);

		switch (PQresultStatus(entity_result)) {
			case PGRES_TUPLES_OK:
				break;
			default:
				// unexpected response
				fprintf(stderr, ERR "%s: %s",
						PQresStatus(PQresultStatus(entity_result)),
						PQresultErrorMessage(entity_result));
				exit(1);
		}

		if (PQntuples(entity_result) == 0) {
			fprintf(stderr, ERR "entity %s not found\n", id);
			exit(1);
		}

		entity = PQgetvalue(entity_result, 0, 0);
		PQclear(entity_result);
	}

	const char *const params[] = {
		entity,
		value
	};

	PGresult *result = PQexecParams(conn,
			"INSERT INTO note (entity, value)"
			"VALUES ($1, $2) "
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

	printf("%s\n", PQgetvalue(result, 0, 0));
	PQclear(result);
}
