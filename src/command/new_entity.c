#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int new_entity(options *options) {
	const char *name = options->identifiers[1];
	if (!name) {
		fprintf(stderr, ERR "no name specified\n");
		return 1;
	}

	mend_entity *entity = mend_new_entity(name);
	if (!entity) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	time_t created = mend_entity_created(entity);
	printf("%s %s %s",
			mend_entity_uid(entity),
			mend_entity_name(entity),
			asctime(gmtime(&created)));
	return 0;
}
