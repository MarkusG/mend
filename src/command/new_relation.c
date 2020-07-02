#include <stdlib.h>

#include "command.h"
#include "../utils.h"

void new_relation(PGconn *conn, options *options) {
	const char *id_a = options->identifiers[1];
	if (!id_a) {
		fprintf(stderr, ERR "not enough arguments\n");
		exit(1);
	}

	const char *id_b = options->identifiers[2];
	if (!id_b) {
		fprintf(stderr, ERR "not enough arguments\n");
		exit(1);
	}

	const char *note = options->identifiers[3];
	const char *entity[2] = { id_a, id_b };

	for (int i = 1; i < 3; ++i) {
		const char *id = options->identifiers[i];
		if (!is_uuid(id)) {
			PGresult *entity_result = PQexecParams(conn,
					"SELECT entity "
					"FROM primary_alias "
					"WHERE alias = $1",
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
				fprintf(stderr, ERR "entity with primary alias %s not found\n", id);
				exit(1);
			}

			entity[i] = PQgetvalue(entity_result, 0, 0);
			PQclear(entity_result);
		}
	}

	PGresult *result;
	if (note) {
		const char *const params[] = {
			entity[0],
			entity[1],
			note
		};

		result = PQexecParams(conn,
				"INSERT INTO relation (entity_a, entity_b, note)"
				"VALUES ($1, $2, $3) "
				"RETURNING uid",
				3,
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
	} else {
		result = PQexecParams(conn,
				"INSERT INTO relation (entity_a, entity_b)"
				"VALUES ($1, $2) "
				"RETURNING uid",
				2,
				NULL,
				entity,
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
	}

	printf("%s\n", PQgetvalue(result, 0, 0));
	PQclear(result);
}
