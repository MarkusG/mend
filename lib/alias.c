#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#include "_mend.h"
#include "../include/mend.h"

const char *mend_alias_uid(
		const mend_alias *alias) {
	return alias->uid;
}

const char *mend_alias_entity_uid(
		const mend_alias *alias) {
	return alias->entity_uid;
}

const char *mend_alias_value(
		const mend_alias *alias) {
	return alias->value;
}

time_t mend_alias_since(
		const mend_alias *alias) {
	return alias->since;
}

const mend_alias *mend_new_alias(
		const char *id,
		mend_id_kind kind,
		const char *name) {
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
			"RETURNING uid::TEXT, entity::TEXT, value, EXTRACT(EPOCH FROM since)::INTEGER",
			2,
			NULL,
			params,
			NULL,
			NULL,
			1);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		PQclear(result);
		return NULL;
	}

	mend_alias *value = malloc(sizeof(mend_alias));
	value->uid = strdup(PQgetvalue(result, 0, 0));
	value->entity_uid = strdup(PQgetvalue(result, 0, 1));
	value->value = strdup(PQgetvalue(result, 0, 2));
	value->since = ntohl(*((long int*)PQgetvalue(result, 0, 3)));
	PQclear(result);
	if (id_converted)
		free((void*)identifier);
	return (const mend_alias*)value;
}

void mend_free_alias(
		const mend_alias *alias) {
	free((void*)alias->uid);
	free((void*)alias->entity_uid);
	free((void*)alias->value);
}

const mend_alias *mend_get_alias(
		const char *id,
		mend_id_kind kind) {
	const char *query;
	switch (kind) {
		case MEND_UUID:
			query =
				"SELECT uid::TEXT, entity::TEXT, value, EXTRACT(EPOCH FROM since)::INTEGER",
				"FROM alias "
				"WHERE uid = $1";
			break;
		case MEND_NAME:
			query =
				"SELECT uid::TEXT, entity::TEXT, value, EXTRACT(EPOCH FROM since)::INTEGER",
				"FROM entity "
				"WHERE name = $1";
			break;
	}

	PGresult *result = PQexecParams(_conn,
			query,
			1,
			NULL,
			&id,
			NULL,
			NULL,
			1);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		PQclear(result);
		return NULL;
	}

	if (PQntuples(result) == 0) {
		_set_error("no such alias %s", id);
		PQclear(result);
		return NULL;
	}

	mend_alias *value = malloc(sizeof(mend_alias));
	value->uid = strdup(PQgetvalue(result, 0, 0));
	value->entity_uid = strdup(PQgetvalue(result, 0, 1));
	value->value = strdup(PQgetvalue(result, 0, 2));
	value->since = ntohl(*((long int*)PQgetvalue(result, 0, 3)));

	PQclear(result);
	return (const mend_alias*)value;
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
