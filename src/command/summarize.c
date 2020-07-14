#include <stdlib.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int summarize(options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		exit(1);
	}

	mend_id_kind kind;
	if (is_uuid(id))
		kind = MEND_UUID;
	else
		kind = MEND_NAME;

	
}
