#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

#include "_mend.h"
#include "../include/mend.h"

const char *mend_entity_uid(mend_entity *entity) {
	return entity->uid;
}

const char *mend_entity_name(mend_entity *entity) {
	return entity->name;
}

time_t mend_entity_created(mend_entity *entity) {
	return entity->created;
}

int mend_uid_from_name(
		const char *name,
		const char **uid) {
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
		PQclear(result);
		return 1;
	}

	if (PQntuples(result) == 0) {
		_set_error("mend_uid_from_name: no entity found");
		PQclear(result);
		return 1;
	}

	*uid = strdup(PQgetvalue(result, 0, 0));
	PQclear(result);
	return 0;
}

mend_entity *mend_new_entity(
		const char *name) {
	// TODO handle UUIDs as UUIDS and not strings
	PGresult *result = PQexecParams(_conn,
			"INSERT INTO entity (name) "
			"VALUES ($1) "
			"RETURNING uid::TEXT, name, EXTRACT(EPOCH FROM created)::INTEGER",
			1,
			NULL,
			&name,
			NULL,
			NULL,
			1);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		PQclear(result);
		return NULL;
	}

	mend_entity *value = malloc(sizeof(mend_entity));
	value->uid = strdup(PQgetvalue(result, 0, 0));
	value->name = strdup(PQgetvalue(result, 0, 1));
	value->created = ntohl(*((long int*)PQgetvalue(result, 0, 2)));

	PQclear(result);
	return value;
}

void mend_free_entity(
		mend_entity *entity) {
	free((void*)entity->uid);
	free((void*)entity->name);
	free((void*)entity);
}

mend_entity **mend_get_entities() {
	PGresult *result = PQexecParams(_conn,
			"SELECT uid::TEXT, name, EXTRACT(EPOCH FROM created)::INTEGER "
			"FROM entity "
			"ORDER BY name",
			0,
			NULL,
			NULL,
			NULL,
			NULL,
			1);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		PQclear(result);
		return NULL;
	}

	mend_entity **ret = malloc((PQntuples(result) + 1) * sizeof(mend_entity*));
	int i;
	for (i = 0; i < PQntuples(result); i++) {
		ret[i] = malloc(sizeof(mend_entity));
		ret[i]->uid = strdup(PQgetvalue(result, i, 0));
		ret[i]->name = strdup(PQgetvalue(result, i, 1));
		ret[i]->created = ntohl(*((long int*)PQgetvalue(result, i, 2)));
	}
	ret[i] = NULL;
	PQclear(result);
	return ret;
}

mend_entity *mend_get_entity(
		const char *id,
		mend_id_kind kind) {
	const char *query;
	switch (kind) {
		case MEND_UUID:
			query =
				"SELECT uid::TEXT, name, EXTRACT(EPOCH FROM created)::INTEGER "
				"FROM entity "
				"WHERE uid = $1";
			break;
		case MEND_NAME:
			query =
				"SELECT uid::TEXT, name, EXTRACT(EPOCH FROM created)::INTEGER "
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
		_set_error("no such entity %s", id);
		PQclear(result);
		return NULL;
	}

	mend_entity *value = malloc(sizeof(mend_entity));
	value->uid = strdup(PQgetvalue(result, 0, 0));
	value->name = strdup(PQgetvalue(result, 0, 1));
	value->created = ntohl(*((long int*)PQgetvalue(result, 0, 2)));

	PQclear(result);
	return value;
}

int mend_remove_entity(
		const char *id,
		mend_id_kind kind) {
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
		PQclear(result);
		return 1;
	}

	if (PQntuples(result) == 0) {
		_set_error("no such entity %s", id);
		PQclear(result);
		return 2;
	}

	PQclear(result);
	return 0;
}
