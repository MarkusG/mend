#ifndef __MEND_H__
#define __MEND_H__
int mend_init(const char*);
int mend_cleanup();

typedef struct entity entity;

entity *new_entity(const char*);
void free_entity(const entity*);

const char **entity_aliases(entity*);
const char *entity_uuid(entity*);

entity **get_entities_from_alias(const char*);
#endif /* ifndef __MEND_H__ */
