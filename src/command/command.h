#ifndef __MEND_COMMAND_H__
#define __MEND_COMMAND_H__
#include <libpq-fe.h>

#include "../options.h"

typedef enum command {
	LIST_ENTITIES,
	CREATE_ENTITY,
	DELETE_ENTITY,
	CREATE_ALIAS,
	DELETE_ALIAS,
	CREATE_ANNOTATION,
	UPDATE_ANNOTATION,
	DELETE_ANNOTATION,
	CREATE_RELATION,
	UPDATE_RELATION,
	DELETE_RELATION
} command;

int list_entities(options*);
int new_entity(options*);
int remove_entity(options*);
int new_alias(options*);
int remove_alias(options*);
int new_note(options*);
int edit_note(options*);
int remove_note(options*);
void new_relation(PGconn*, options*);
void edit_relation(PGconn*, options*);
void remove_relation(PGconn*, options*);
#endif /* ifndef __MEND_COMMAND_H__ */
