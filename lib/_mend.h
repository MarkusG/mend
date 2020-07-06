#ifndef ___MEND_H__
#define ___MEND_H__
#include <libpq-fe.h>
#include <time.h>
extern PGconn *_conn;
extern char *_error;
void _set_error(const char*, ...);

struct mend_entity {
	const char *uid;
	const char *name;
	time_t created;
};

struct mend_alias {
	const char *uid;
	const char *entity_uid;
	const char *value;
	time_t since;
};

struct mend_note {
	const char *uid;
	const char *entity_uid;
	const char *value;
	time_t created;
	time_t updated;
};
#endif /* ifndef ___MEND_H__ */
