#include <stdlib.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int list_aliases(options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		return 1;
	}

	mend_id_kind kind;
	if (is_uuid(id))
		kind = MEND_UUID;
	else
		kind = MEND_NAME;

	const mend_entity *entity = mend_get_entity(id, kind);
	const mend_alias **aliases = mend_get_aliases(id, kind);
	if (!aliases) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}
	print_entity(entity, options);
	putchar('\n');
	int i = 0;
	const mend_alias *alias;
	while ((alias = aliases[i])) {
		printf("%s\n", mend_alias_value(alias));
		mend_free_alias(alias);
		++i;
	}
	free((void*)aliases);
	mend_free_entity(entity);
	return 0;
}
