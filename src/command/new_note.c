#include <stdlib.h>

#include "../../include/mend.h"
#include "../utils.h"
#include "command.h"

int new_note(options *options) {
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

	const char *value;
	int edit_ret = edit_path("/tmp/mend_note", &value);
	if (edit_ret == 1)
		return 1;
	else if (edit_ret == 2) {
		fprintf(stderr, ERR "empty note\n");
		return 1;
	}

	const mend_note *note = mend_new_note(id, kind, value);
	if (!note) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	printf("%s\n", mend_note_uid(note));
	free((void*)value);
	mend_free_note(note);
	return 0;
}
