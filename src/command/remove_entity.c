#include <stdlib.h>

#include "../../include/mend.h"
#include "../utils.h"
#include "command.h"

int remove_entity(options *options) {
	if (!options->identifiers[1]) {
		fprintf(stderr, ERR "no identifier specified\n");
		return 1;
	}

	int i = 1;
	const char *id;
	int result = 0;
	mend_id_kind kind;
	while ((id = options->identifiers[i])) {
		if (is_uuid(id))
			kind = MEND_UUID;
		else
			kind = MEND_NAME;
		if (mend_remove_entity(id, kind)) {
			fprintf(stderr, ERR "%s\n", mend_error());
			result = 1;
		}
		++i;
	}

	return result;
}
