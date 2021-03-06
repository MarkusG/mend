#include <stdlib.h>

#include "../../include/mend.h"
#include "../utils.h"
#include "command.h"

int list_entities(options *options) {
	if (options->identifiers[1]) {
		int i = 1;
		const char *id;
		mend_id_kind kind;
		while ((id = options->identifiers[i])) {
			if (is_uuid(id))
				kind = MEND_UUID;
			else
				kind = MEND_NAME;
			const mend_entity *entity = mend_get_entity(id, kind);
			if (!entity) {
				fprintf(stderr, ERR "%s\n", mend_error());
				return 1;
			}

			print_entity(entity, options);
			mend_free_entity(entity);
			++i;
		}
	} else {
		const mend_entity **entities = mend_get_entities();
		if (!entities) {
			fprintf(stderr, ERR "%s\n", mend_error());
			return 1;
		}
		int i = 0;
		const mend_entity *entity;
		while ((entity = entities[i])) {
			print_entity(entity, options);
			mend_free_entity(entity);
			i++;
		}
		free((void*)entities);
	}
	return 0;
}
