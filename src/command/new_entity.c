#include <stdlib.h>

#include "../../include/mend.h"
#include "../utils.h"
#include "command.h"

int new_entity(options *options) {
	const char *name = options->identifiers[1];
	if (!name) {
		fprintf(stderr, ERR "no name specified\n");
		return 1;
	}

	const mend_entity *entity = mend_new_entity(name);
	if (!entity) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	printf("%s\n", mend_entity_uid(entity));
	mend_free_entity(entity);
	return 0;
}
