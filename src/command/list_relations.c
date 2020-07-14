#include <stdlib.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int list_relations(options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		return 1;
	}

	mend_id_kind kind;
	if (is_uuid(id))
		kind = MEND_UUID;
	else
		kind = MEND_NAME;

	const mend_entity *this = mend_get_entity(id, kind);
	const mend_entity **entities = mend_get_related_entities(id, kind);
	if (!entities || !this) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}
	print_entity(this, options);
	putchar('\n');
	int i = 0;
	const mend_entity *entity;
	while ((entity = entities[i])) {
		print_entity(entity, options);
		mend_free_entity(entity);
		++i;
	}
	free((void*)entities);
	mend_free_entity(this);
	return 0;
}
