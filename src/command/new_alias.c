#include <stdlib.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int new_alias(options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		exit(1);
	}

	const char *value = options->identifiers[2];
	if (!value) {
		fprintf(stderr, ERR "no value specified\n");
		exit(1);
	}

	mend_id_kind kind;
	if (is_uuid(id))
		kind = MEND_UUID;
	else
		kind = MEND_NAME;

	const char *uid;
	if (mend_new_alias(id, kind, value, &uid)) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	printf("%s\n", uid);
	return 0;
}
