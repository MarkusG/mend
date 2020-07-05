#include <stdlib.h>

#include "command.h"
#include "../utils.h"

void list_entities(PGconn *conn, options *options) {
	if (options->identifiers[1]) {
		int i = 1;
		const char *id;
		while ((id = options->identifiers[i])) {
			PGresult *result = PQexecParams(conn,
					"SELECT * "
					"FROM primary_alias "
					"WHERE entity::TEXT = $1 "
					"OR alias = $1 "
					"ORDER BY alias",
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

			
			int n_tuples = PQntuples(result);
			for (int i = 0; i < n_tuples; i++) {
				printf("%s %s\n",
						PQgetvalue(result, i, 0),
						PQgetvalue(result, i, 1));
			}
			++i;
			PQclear(result);
		}
	} else {
		PGresult *result = PQexecParams(conn,
				"SELECT * "
				"FROM primary_alias "
				"ORDER BY alias",
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
				fprintf(stderr, ERR "%s: %s",
						PQresStatus(PQresultStatus(result)),
						PQresultErrorMessage(result));
				exit(1);
		}
		int n_tuples = PQntuples(result);
		for (int i = 0; i < n_tuples; i++) {
			printf("%s %s\n",
					PQgetvalue(result, i, 0),
					PQgetvalue(result, i, 1));
		}
		PQclear(result);
	}
}
