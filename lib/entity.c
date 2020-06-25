#include <stdlib.h>
#include <string.h>

#include <libpq-fe.h>

#include "../include/mend.h"
#include "mend.h"

struct entity {
	const char *uuid;
	const char **aliases;
};

entity *new_entity(const char *uuid) {
	entity *result = (entity*)malloc(sizeof(entity));
	result->uuid = uuid;
	result->aliases = NULL;
	return result;
}

void free_entity(const entity* entity) {
	free((void*)entity->uuid);
	if (entity->aliases) {
		int i = 0;
		while (entity->aliases[i]) {
			free((void*)entity->aliases[i]);
		}
		free(entity->aliases);
	}
	free((void*)entity);
}

const char *entity_uuid(entity *entity) {
	return entity->uuid;
}

const char **entity_aliases(entity *entity) {
	if (!entity->aliases) {
		PGresult *result = PQexecParams(
				_conn,
				"SELECT a.value "
				"FROM alias AS a "
				"LEFT JOIN entity AS e on a.entity = e.uid "
				"WHERE e.uid = $1;",
				1,
				NULL,
				&(entity->uuid),
				NULL,
				NULL,
				0);

		switch (PQresultStatus(result)) {
			case PGRES_TUPLES_OK:
				break;
			case PGRES_COMMAND_OK:
				return NULL;
			default:
				fprintf(stderr, "%s\n%s\n",
						PQresStatus(PQresultStatus(result)),
						PQresultErrorMessage(result));
		}

		int n_tuples = PQntuples(result);
		entity->aliases = (const char**)malloc((1 + n_tuples) * sizeof(char*));
		int i;
		for (i = 0; i < n_tuples; i++) {
			entity->aliases[i] = strdup(PQgetvalue(result, i, 0));
		}
		entity->aliases[i] = NULL;

		PQclear(result);
	}
	return entity->aliases;
}

entity **get_entities_from_alias(const char *alias) {
	PGresult *result = PQexecParams(
			_conn,
			"SELECT e.uid "
			"FROM entity AS e "
			"LEFT JOIN alias AS a ON e.uid = a.entity "
			"WHERE a.value = $1;",
			1, // parameter count
			NULL, // parameter types; NULL to infer
			&alias, // parameters
			NULL, // lengths; not needed for strings
			NULL, // formats; NULL for all strings
			0); // result format; 0 for text

	switch (PQresultStatus(result)) {
		case PGRES_TUPLES_OK:
			break;
		case PGRES_COMMAND_OK:
			return NULL;
		default:
			fprintf(stderr, "%s\n%s\n",
					PQresStatus(PQresultStatus(result)),
					PQresultErrorMessage(result));
	}

	int n_tuples = PQntuples(result);
	entity **entities = (entity**)malloc((1 + n_tuples) * sizeof(entity*));
	int i;
	for (i = 0; i < n_tuples; i++) {
		entities[i] = new_entity(strdup(PQgetvalue(result, i, 0)));
	}
	entities[i] = NULL;

	PQclear(result);
	return entities;
}
