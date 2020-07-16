#include <stdlib.h>

#include "../../include/mend.h"
#include "../utils.h"
#include "command.h"

int remove_relation(options *options) {
	if (!options->identifiers[1]) {
		fprintf(stderr, ERR "no identifier specified\n");
		return 1;
	}

	int i = 1;
	const char *id;
	int result = 0;
	while ((id = options->identifiers[i])) {
		if (!is_uuid(id)) {
			fprintf(stderr, ERR "\"%s\" is not a UUID\n", id);
			++i;
			continue;
		}

		if (mend_remove_relation(id)) {
			fprintf(stderr, ERR "%s\n", mend_error());
			result = 1;
		}

		++i;
	}

	return result;
}
