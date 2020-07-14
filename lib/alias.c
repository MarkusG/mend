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
			identifier = mend_uid_from_name(id);
			id_converted = 1;
			break;
	}

	if (!identifier) {
		_set_error("no such entity %s", id);
		return NULL;
	}

	const char *params[] = {
		identifier,
		name
	};

	PGresult *result = PQexecParams(_conn,
			"INSERT INTO alias (entity, value) "
			"VALUES ($1, $2) "
			"RETURNING "
			"	uid::TEXT, "
			"	entity::TEXT, "
			"	value, "
			"	EXTRACT(EPOCH FROM since)::INTEGER",
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

	mend_alias *ret = malloc(sizeof(mend_alias));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->entity_uid = strdup(PQgetvalue(result, 0, 1));
	ret->value = strdup(PQgetvalue(result, 0, 2));
	ret->since = ntohl(*((time_t*)PQgetvalue(result, 0, 3)));
	PQclear(result);
	if (id_converted)
		free((void*)identifier);
	return (const mend_alias*)ret;
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
				"SELECT "
				"	uid::TEXT, "
				"	entity::TEXT, "
				"	value, "
				"	EXTRACT(EPOCH FROM since)::INTEGER "
				"FROM alias "
				"WHERE uid = $1";
			break;
		case MEND_NAME:
			query =
				"SELECT "
				"	uid::TEXT, "
				"	entity::TEXT, "
				"	value, "
				"	EXTRACT(EPOCH FROM since)::INTEGER "
				"FROM alias "
				"WHERE value = $1";
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

	mend_alias *ret = malloc(sizeof(mend_alias));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->entity_uid = strdup(PQgetvalue(result, 0, 1));
	ret->value = strdup(PQgetvalue(result, 0, 2));
	ret->since = ntohl(*((time_t*)PQgetvalue(result, 0, 3)));

	PQclear(result);
	return (const mend_alias*)ret;
}

const mend_alias **mend_get_aliases(
		const char *id, 
		mend_id_kind kind) {
	const char *identifier;
	int id_converted = 0;
	switch (kind) {
		case MEND_UUID:
			identifier = id;
			break;
		case MEND_NAME:
			identifier = mend_uid_from_name(id);
			id_converted = 1;
			break;
	}

	if (!identifier) {
		_set_error("no such entity %s", id);
		return NULL;
	}

	PGresult *result = PQexecParams(_conn,
			"SELECT "
			"	uid::TEXT, "
			"	entity::TEXT, "
			"	value, "
			"	EXTRACT(EPOCH FROM since)::INTEGER "
			"FROM alias "
			"WHERE entity = $1"
			"ORDER BY since DESC",
			1,
			NULL,
			&identifier,
			NULL,
			NULL,
			1);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		PQclear(result);
		return NULL;
	}

	int n_tuples = PQntuples(result);
	if (n_tuples == 0) {
		_set_error("no aliases for entity %s", identifier);
		PQclear(result);
		return NULL;
	}

	mend_alias **ret = malloc((n_tuples + 1) * sizeof(mend_alias*));
	int i;
	for (i = 0; i < n_tuples; ++i) {
		mend_alias *alias = malloc(sizeof(mend_alias));
		alias->uid = strdup(PQgetvalue(result, i, 0));
		alias->entity_uid = strdup(PQgetvalue(result, i, 1));
		alias->value = strdup(PQgetvalue(result, i, 2));
		alias->since = ntohl(*((time_t*)PQgetvalue(result, i, 3)));
		ret[i] = alias;
	}
	ret[i] = NULL;
	if (id_converted)
		free((void*)identifier);
	PQclear(result);
	return (const mend_alias**)ret;
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

	PQclear(result);
	return 0;
}
