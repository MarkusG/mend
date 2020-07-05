#include <stdlib.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int new_entity(options *options) {
	const char *name = options->identifiers[1];
	if (!name) {
		fprintf(stderr, ERR "no name specified\n");
		return 1;
	}

	const char *uid;
	if (mend_new_entity(name, &uid)) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	printf("%s\n", uid);
	return 0;
}
