#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mend.h"
#include "_mend.h"

const char *mend_note_uid(
		const mend_note *note) {
	return note->uid;
}

const char *mend_note_entity_uid(
		const mend_note *note) {
	return note->entity_uid;
}

const char *mend_note_value(
		const mend_note *note) {
	return note->value;
}

time_t mend_note_created(const mend_note *note) {
	return note->created;
}

time_t mend_note_updated(const mend_note *note) {
	return note->updated;
}

const mend_note *mend_new_note(
		const char *id,
		mend_id_kind kind,
		const char *value) {
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
		value
	};

	PGresult *result = PQexecParams(_conn,
			"INSERT INTO note (entity, value) "
			"VALUES ($1, $2) "
			"RETURNING "
			"	uid::TEXT, "
			"	entity::TEXT, "
			"	value, "
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
		if (id_converted)
			free((void*)identifier);
		PQclear(result);
		return NULL;
	}

	
	mend_note *ret = malloc(sizeof(mend_note));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->entity_uid = strdup(PQgetvalue(result, 0, 1));
	ret->value = strdup(PQgetvalue(result, 0, 2));
	ret->created = ntohl(*((time_t*)PQgetvalue(result, 0, 3)));
	ret->updated = ntohl(*((time_t*)PQgetvalue(result, 0, 4)));
	PQclear(result);
	if (id_converted)
		free((void*)identifier);
	return (const mend_note*)ret;
}

void mend_free_note(
		const mend_note *note) {
	free((void*)note->uid);
	free((void*)note->entity_uid);
	free((void*)note->value);
	free((void*)note);
}


const mend_note *mend_get_note(
		const char *uid) {
	PGresult *result = PQexecParams(_conn,
			"SELECT "
			"	uid::TEXT, "
			"	entity::TEXT, "
			"	value, "
			"	EXTRACT(EPOCH FROM created)::INTEGER, "
			"	EXTRACT(EPOCH FROM updated)::INTEGER "
			"FROM note "
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
		_set_error("no such note %s", uid);
		PQclear(result);
		return NULL;
	}

	mend_note *ret = malloc(sizeof(mend_note));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->entity_uid = strdup(PQgetvalue(result, 0, 1));
	ret->value = strdup(PQgetvalue(result, 0, 2));
	ret->created = ntohl(*((time_t*)PQgetvalue(result, 0, 3)));
	ret->updated = ntohl(*((time_t*)PQgetvalue(result, 0, 4)));
	PQclear(result);
	return (const mend_note*)ret;
}

const mend_note **mend_get_notes(
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
			"	EXTRACT(EPOCH FROM created)::INTEGER, "
			"	EXTRACT(EPOCH FROM updated)::INTEGER "
			"FROM note "
			"WHERE entity = $1",
			1,
			NULL,
			&identifier,
			NULL,
			NULL,
			1);

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		_set_error("libpq: %s", PQresultErrorMessage(result));
		if (id_converted)
			free((void*)identifier);
		PQclear(result);
		return NULL;
	}

	int n_tuples = PQntuples(result);
	if (n_tuples == 0) {
		_set_error("no notes for entity %s", identifier);
		if (id_converted)
			free((void*)identifier);
		PQclear(result);
		return NULL;
	}

	mend_note **ret = malloc((n_tuples + 1) * sizeof(mend_note*));
	int i;
	for (i = 0; i < n_tuples; ++i) {
		mend_note *note = malloc(sizeof(mend_note));
		note->uid = strdup(PQgetvalue(result, i, 0));
		note->entity_uid = strdup(PQgetvalue(result, i, 1));
		note->value = strdup(PQgetvalue(result, i, 2));
		note->created = ntohl(*((time_t*)PQgetvalue(result, i, 3)));
		note->updated = ntohl(*((time_t*)PQgetvalue(result, i, 4)));
		ret[i] = note;
	}
	ret[i] = NULL;
	if (id_converted)
		free((void*)identifier);
	PQclear(result);
	return (const mend_note**)ret;
}

const mend_note *mend_edit_note(
		const char *uid,
		const char *value) {
	const char *params[] = {
		value,
		uid
	};

	PGresult *result = PQexecParams(_conn,
			"UPDATE note "
			"SET value = $1 "
			"WHERE uid = $2 "
			"RETURNING "
			"	uid::TEXT, "
			"	entity::TEXT, "
			"	value, "
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
		_set_error("no such note %s", uid);
		PQclear(result);
		return NULL;
	}

	mend_note *ret = malloc(sizeof(mend_note));
	ret->uid = strdup(PQgetvalue(result, 0, 0));
	ret->entity_uid = strdup(PQgetvalue(result, 0, 1));
	ret->value = strdup(PQgetvalue(result, 0, 2));
	ret->created = ntohl(*((time_t*)PQgetvalue(result, 0, 3)));
	ret->updated = ntohl(*((time_t*)PQgetvalue(result, 0, 4)));
	PQclear(result);
	return (const mend_note*)ret;
}

int mend_remove_note(
		const char *uid) {
	PGresult *result = PQexecParams(_conn,
			"DELETE FROM note "
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
		_set_error("no such note %s", uid);
		PQclear(result);
		return 2;
	}

	PQclear(result);
	return 0;
}
