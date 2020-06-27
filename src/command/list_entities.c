#include <stdlib.h>

#include "command.h"
#include "../utils.h"

void list_entities(PGconn *conn, options *options) {
	if (options->identifiers[1]) {
		int i = 1;
		const char *id;
		while ((id = options->identifiers[i])) {
			const char *query;
			if (is_uuid(id)) {
				query = 
					"SELECT * "
					"FROM primary_alias "
					"WHERE entity = $1";
			} else {
				query = 
					"SELECT * "
					"FROM primary_alias "
					"WHERE alias = $1";
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
					// intended behavior
					break;
				case PGRES_COMMAND_OK:
					// we got no data back
					fprintf(stderr, "Identifier not found: %s\n", id);
					exit(1);
				default:
					// unexpected response
					fprintf(stderr, "%s: %s",
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
		}
	} else {
		PGresult *result = PQexecParams(conn,
				"SELECT * "
				"FROM primary_alias",
				0,
				NULL,
				NULL,
				NULL,
				NULL,
				0);
		switch (PQresultStatus(result)) {
			case PGRES_TUPLES_OK:
				// intended behavior
				break;
			case PGRES_COMMAND_OK:
				// we got no data back
				break;
			default:
				// unexpected response
				fprintf(stderr, "%s: %s",
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
	}
}
