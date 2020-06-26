#ifndef __MEND_COMMAND_H__
#define __MEND_COMMAND_H__
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

typedef struct optionss {
	command command;
	char **identifiers;
} options;

void list_entities(options*);
int parse_options(options*, int, char*[]);
#endif /* ifndef __MEND_COMMAND_H__ */
