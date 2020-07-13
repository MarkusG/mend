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
const char *mend_uid_from_name(const char*);

typedef struct mend_alias mend_alias;
const mend_alias *mend_new_alias(const char*, mend_id_kind, const char*);
void mend_free_alias(const mend_alias*);

const char *mend_alias_uid(const mend_alias *alias);
const char *mend_alias_value(const mend_alias *alias);
const char *mend_alias_entity_uid(const mend_alias *alias);
time_t mend_alias_since(const mend_alias *alias);

const mend_alias *mend_get_alias(const char*, mend_id_kind);
int mend_remove_alias(const char*, mend_id_kind);

typedef struct mend_note mend_note;
const mend_note *mend_new_note(const char*, mend_id_kind, const char*);
void mend_free_note(const mend_note*);

const char *mend_note_uid(const mend_note*);
const char *mend_note_entity_uid(const mend_note*);
const char *mend_note_value(const mend_note*);
time_t mend_note_created(const mend_note*);
time_t mend_note_updated(const mend_note*);

const mend_note *mend_get_note(const char*);
const mend_note *mend_edit_note(const char*, const char*);
int mend_remove_note(const char*);

typedef struct mend_relation mend_relation;
const mend_relation *mend_new_relation(const char*[], mend_id_kind[], const char*);
void mend_free_relation(const mend_relation*);

const char *mend_relation_uid(const mend_relation*);
const char *mend_relation_entity_a_uid(const mend_relation*);
const char *mend_relation_entity_b_uid(const mend_relation*);
const char *mend_relation_note(const mend_relation*);
time_t mend_relation_created(const mend_relation*);
time_t mend_relation_updated(const mend_relation*);

const mend_relation *mend_get_relation(const char*);
const mend_relation *mend_edit_relation(const char*, const char*);
int mend_remove_relation(const char*);
#endif /* ifndef __MEND_H__ */
