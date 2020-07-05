#include <string.h>

#include "_mend.h"
#include "../include/mend.h"

int mend_uid_from_name(const char *name, const char **uid) {
	PGresult *result = PQexecParams(_conn,
				"SELECT uid "
				"FROM entity "
				"WHERE name = $1",
				1,
				NULL,
				&name,
				NULL,
				NULL,
				0);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		return 1;
	}

	if (PQntuples(result) == 0) {
		_set_error("mend_uid_from_name: no entity found");
		return 1;
	}

	*uid = strdup(PQgetvalue(result, 0, 0));
	PQclear(result);
	return 0;
}

int mend_new_entity(const char *name, const char **uid) {
	PGresult *result = PQexecParams(_conn,
			"INSERT INTO entity (name) "
			"VALUES ($1) "
			"RETURNING uid",
			1,
			NULL,
			&name,
			NULL,
			NULL,
			0);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		return 1;
	}

	if (uid)
		*uid = strdup(PQgetvalue(result, 0, 0));

	PQclear(result);
	return 0;
}

int mend_remove_entity(const char *id, mend_id_kind kind) {
	const char *query;
	switch (kind) {
		case MEND_UUID:
			query =
				"DELETE FROM entity "
				"WHERE uid = $1 "
				"RETURNING uid";
			break;
		case MEND_NAME:
			query =
				"DELETE FROM entity "
				"WHERE name = $1 "
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
		return 1;
	}

	if (PQntuples(result) == 0) {
		_set_error("no such entity %s", id);
		return 2;
	}

	PQclear(result);
	return 0;
}
