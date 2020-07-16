#include <stdlib.h>

#include "../../include/mend.h"
#include "../utils.h"
#include "command.h"

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
		char a_timebuf[20];
		time_t since = localize(mend_alias_since(alias));
		strftime(a_timebuf, sizeof(a_timebuf), "%F %R", localtime(&since));
		printf("since %s: %s\n", a_timebuf, mend_alias_value(alias));
		mend_free_alias(alias);
		++i;
	}
	free((void*)aliases);
	mend_free_entity(entity);
	return 0;
}
