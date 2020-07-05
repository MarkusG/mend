#include <stdlib.h>
#include <string.h>

#include "_mend.h"
#include "../include/mend.h"

int mend_new_alias(
		const char *id,
		mend_id_kind kind,
		const char *name,
		const char **uid) {
	const char *identifier;
	int id_converted = 0;
	switch (kind) {
		case MEND_UUID:
			identifier = id;
			break;
		case MEND_NAME:
			mend_uid_from_name(id, &identifier);
			id_converted = 1;
			break;
	}

	const char *params[] = {
		identifier,
		name
	};

	PGresult *result = PQexecParams(_conn,
			"INSERT INTO alias (entity, value) "
			"VALUES ($1, $2) "
			"RETURNING uid",
			2,
			NULL,
			params,
			NULL,
			NULL,
			0);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		return 1;
	}

	*uid = strdup(PQgetvalue(result, 0, 0));
	PQclear(result);
	if (id_converted)
		free((void*)identifier);
	return 0;
}

int mend_remove_alias(
		const char *id,
		mend_id_kind kind) {
	const char *query;
	switch (kind) {
		case MEND_UUID:
			query =
				"DELETE FROM alias "
				"WHERE uid = $1 "
				"RETURNING uid";
			break;
		case MEND_NAME:
			query =
				"DELETE FROM alias "
				"WHERE value = $1"
				"RETURNING uid";
			break;
	}

	PGresult *result = PQexecParams(_conn,
			query,
			1,
			NULL,
			&id,
			NULL,
			NULL,
			0);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		PQclear(result);
		return 1;
	}

	if (PQntuples(result) == 0) {
		_set_error("no such alias %s", id);
		PQclear(result);
		return 2;
	}

	return 0;
}
