#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "_mend.h"
#include "../include/mend.h"

const char *mend_relation_uid(
		const mend_relation *relation) {
	return relation->uid;
}

const char *mend_relation_entity_a_uid(
		const mend_relation *relation) {
	return relation->a_uid;
}

const char *mend_relation_entity_b_uid(
		const mend_relation *relation) {
	return relation->b_uid;
}

const char *mend_relation_note(
		const mend_relation *relation) {
	return relation->note;
}

time_t mend_relation_created(
		const mend_relation *relation) {
	return relation->created;
}

time_t mend_relation_updated(
		const mend_relation *relation) {
	return relation->updated;
}

const mend_relation *mend_new_relation(
		const char *entities[],
		mend_id_kind kinds[], 
		const char *note) {
	const char *identifiers[2];
	int id_converted[2] = { 0, 0 };
	for (int i = 0; i < 2; i++) {
		const char *id = entities[i];
		switch (kinds[i]) {
			case MEND_UUID:
				identifiers[i] = id;
				break;
			case MEND_NAME:
				identifiers[i] = mend_uid_from_name(id);
				id_converted[i] = 1;
				break;
		}

		if (!identifiers[i]) {
			_set_error("no such entity %s", id);
			return NULL;
		}
	}

	const char *params[] = {
		identifiers[0],
		identifiers[1],
		note
	};

	PGresult *result = PQexecParams(_conn,
			"INSERT INTO relation (entity_a, entity_b, note) "
			"VALUES ($1, $2, $3) "
			"RETURNING uid::TEXT, entity_a::TEXT, entity_b::TEXT, note, "
			"	EXTRACT(EPOCH FROM created)::INTEGER, "
			"	EXTRACT(EPOCH FROM updated)::INTEGER",
			3,
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

	mend_relation *ret = malloc(sizeof(mend_relation));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->a_uid = strdup(PQgetvalue(result, 0, 1));
	ret->b_uid = strdup(PQgetvalue(result, 0, 2));
	ret->note = strdup(PQgetvalue(result, 0, 3));
	ret->created = ntohl(*((time_t*)PQgetvalue(result, 0, 4)));
	ret->updated = ntohl(*((time_t*)PQgetvalue(result, 0, 5)));
	PQclear(result);
	for (int i = 0; i < 2; i++) {
		if (id_converted[i])
			free((void*)identifiers[i]);
	}
	return (const mend_relation*)ret;
}

void mend_free_relation(
		const mend_relation *relation) {
	free((void*)relation->uid);
	free((void*)relation->a_uid);
	free((void*)relation->b_uid);
	free((void*)relation->note);
	free((void*)relation);
}

const mend_relation *mend_get_relation(
		const char *uid) {
	PGresult *result = PQexecParams(_conn,
			"SELECT uid::TEXT, entity_a::TEXT, entity_b::TEXT, note, "
			"	EXTRACT(EPOCH FROM created)::INTEGER, "
			"	EXTRACT(EPOCH FROM updated)::INTEGER "
			"FROM relation "
			"WHERE uid = $1",
			1,
			NULL,
			&uid,
			NULL,
			NULL,
			1);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		PQclear(result);
		return NULL;
	}

	if (PQntuples(result) == 0) {
		_set_error("no such relation %s", uid);
		PQclear(result);
		return NULL;
	}

	mend_relation *ret = malloc(sizeof(mend_relation));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->a_uid = strdup(PQgetvalue(result, 0, 1));
	ret->b_uid = strdup(PQgetvalue(result, 0, 2));
	ret->note = strdup(PQgetvalue(result, 0, 3));
	ret->created = ntohl(*((time_t*)PQgetvalue(result, 0, 4)));
	ret->updated = ntohl(*((time_t*)PQgetvalue(result, 0, 5)));
	PQclear(result);
	return (const mend_relation*)ret;
}

const mend_relation *mend_edit_relation(
		const char *uid,
		const char *value) {
	const char *params[] = {
		value,
		uid
	};

	PGresult *result = PQexecParams(_conn,
			"UPDATE relation "
			"SET note = $1 "
			"WHERE uid = $2 "
			"RETURNING uid::TEXT, entity_a::TEXT, entity_b::TEXT, note, "
			"	EXTRACT(EPOCH FROM created)::INTEGER, "
			"	EXTRACT(EPOCH FROM updated)::INTEGER",
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
	
	if (PQntuples(result) == 0) {
		_set_error("no such relation %s", uid);
		PQclear(result);
		return NULL;
	}

	mend_relation *ret = malloc(sizeof(mend_relation));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->a_uid = strdup(PQgetvalue(result, 0, 1));
	ret->b_uid = strdup(PQgetvalue(result, 0, 2));
	ret->note = strdup(PQgetvalue(result, 0, 3));
	ret->created = ntohl(*((time_t*)PQgetvalue(result, 0, 4)));
	ret->updated = ntohl(*((time_t*)PQgetvalue(result, 0, 5)));
	PQclear(result);
	return (const mend_relation*)ret;
}

int mend_remove_relation(
		const char *uid) {
	PGresult *result = PQexecParams(_conn,
			"DELETE FROM relation "
			"WHERE uid = $1 "
			"RETURNING uid",
			1,
			NULL,
			&uid,
			NULL,
			NULL,
			0);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		PQclear(result);
		return 1;
	}

	if (PQntuples(result) == 0) {
		_set_error("no such relation %s", uid);
		PQclear(result);
		return 2;
	}

	PQclear(result);
	return 0;
}
