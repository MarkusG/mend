#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int edit_relation(options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		return 1;
	} else if (!is_uuid(id)) {
		fprintf(stderr, ERR "argument must be a UUID\n");
		return 1;
	}

	const char *value = options->identifiers[2];
	if (!value) {
		fprintf(stderr, ERR "no value specified\n");
		return 1;
	}

	const mend_relation *relation = mend_edit_relation(id, value);
	if (!relation) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	printf("%s\n", mend_relation_uid(relation));
	mend_free_relation(relation);

	return 0;
}
