#ifndef __MEND_H__
#define __MEND_H__
#include <time.h>
typedef enum mend_id_kind {
	MEND_UUID,
	MEND_NAME
} mend_id_kind;

const char *mend_error();

int mend_init(const char*);
int mend_cleanup();

typedef struct mend_entity mend_entity;
const mend_entity *mend_new_entity(const char*);
void mend_free_entity(const mend_entity*);

const char *mend_entity_uid(const mend_entity*);
const char *mend_entity_name(const mend_entity*);
time_t mend_entity_created(const mend_entity*);

const mend_entity **mend_get_entities();
const mend_entity *mend_get_entity(const char*, mend_id_kind);

int mend_remove_entity(const char*, mend_id_kind);
int mend_uid_from_name(const char*, const char**);

typedef struct mend_alias mend_alias;
const mend_alias *mend_new_alias(const char*, mend_id_kind, const char*);
void mend_free_alias(const mend_alias*);

const char *mend_alias_uid(const mend_alias *alias);
const char *mend_alias_name(const mend_alias *alias);
const char *mend_alias_entity_uid(const mend_alias *alias);
time_t mend_alias_since(const mend_alias *alias);

int mend_remove_alias(const char*, mend_id_kind);
#endif /* ifndef __MEND_H__ */
