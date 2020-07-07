#include <stdlib.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int new_relation(options *options) {
	const char *id[2];
	mend_id_kind kind[2];
	for (int i = 0; i < 2; i++) {
		if (!options->identifiers[i + 1]) {
			fprintf(stderr, ERR "not enough arguments\n");
			return 1;
		} else {
			id[i] = options->identifiers[i + 1];
			if (is_uuid(id[i]))
				kind[i] = MEND_UUID;
			else
				kind[i] = MEND_NAME;
		}
	}

	const char *note = options->identifiers[3];
	
	const mend_relation *relation = mend_new_relation(id, kind, note);
	if (!relation) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	printf("%s\n", mend_relation_uid(relation));
	mend_free_relation(relation);
	return 0;
}
