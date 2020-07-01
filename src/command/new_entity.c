#include <stdlib.h>

#include "command.h"
#include "../utils.h"

void create_entity(PGconn *conn, options *options) {
	PGresult *result = PQexecParams(conn,
			"INSERT INTO entity "
			"DEFAULT VALUES "
			"RETURNING uid",
			0,
			NULL,
			NULL,
			NULL,
			NULL,
			0);

	switch (PQresultStatus(result)) {
		case PGRES_TUPLES_OK:
			break;
		default:
			// unexpected response
			fprintf(stderr, "%s: %s",
					PQresStatus(PQresultStatus(result)),
					PQresultErrorMessage(result));
			exit(1);
	}

	char *uid = PQgetvalue(result, 0, 0);
	if (options->identifiers[1]) {
		const char *const params[] = { uid, options->identifiers[1] };
		PGresult *alias_result = PQexecParams(conn,
				"INSERT INTO alias (entity, value, precedence) "
				"VALUES ($1, $2, 0)",
				2,
				NULL,
				params,
				NULL,
				NULL,
				0);

		switch (PQresultStatus(alias_result)) {
			case PGRES_COMMAND_OK:
				break;
			default:
				// unexpected response
				fprintf(stderr, "%s: %s",
						PQresStatus(PQresultStatus(alias_result)),
						PQresultErrorMessage(alias_result));
				exit(1);
		}
		PQclear(alias_result);
	}

	printf("%s\n", uid);
	PQclear(result);
}
