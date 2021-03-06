#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/mend.h"
#include "_mend.h"

const char *mend_entity_uid(
		const mend_entity *entity) {
	return entity->uid;
}

const char *mend_entity_name(
		const mend_entity *entity) {
	return entity->name;
}

time_t mend_entity_created(
		const mend_entity *entity) {
	return entity->created;
}

const char *mend_uid_from_name(
		const char *name) {
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
		return NULL;
	}

	if (PQntuples(result) == 0) {
		_set_error("mend_uid_from_name: no entity found");
		PQclear(result);
		return NULL;
	}

	const char *uid = strdup(PQgetvalue(result, 0, 0));
	PQclear(result);
	return uid;
}

const mend_entity *mend_new_entity(
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

	mend_entity *ret = malloc(sizeof(mend_entity));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->name = strdup(PQgetvalue(result, 0, 1));
	ret->created = ntohl(*((time_t*)PQgetvalue(result, 0, 2)));

	PQclear(result);
	return (const mend_entity*)ret;
}

void mend_free_entity(
		const mend_entity *entity) {
	free((void*)entity->uid);
	free((void*)entity->name);
	free((void*)entity);
}

const mend_entity **mend_get_entities() {
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
		mend_entity *entity = malloc(sizeof(mend_entity));
		entity->uid = strdup(PQgetvalue(result, i, 0));
		entity->name = strdup(PQgetvalue(result, i, 1));
		entity->created = ntohl(*((time_t*)PQgetvalue(result, i, 2)));
		ret[i] = entity;
	}
	ret[i] = NULL;
	PQclear(result);
	return (const mend_entity**)ret;
}

const mend_entity *mend_get_entity(
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

	mend_entity *ret = malloc(sizeof(mend_entity));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->name = strdup(PQgetvalue(result, 0, 1));
	ret->created = ntohl(*((time_t*)PQgetvalue(result, 0, 2)));

	PQclear(result);
	return (const mend_entity*)ret;
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
