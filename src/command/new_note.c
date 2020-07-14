#include <stdlib.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

// TODO open the user's editor to create the note
int new_note(options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		return 1;
	}

	const char *value = options->identifiers[2];
	if (!value) {
		fprintf(stderr, ERR "no value specified\n");
		return 1;
	}

	mend_id_kind kind;
	if (is_uuid(id))
		kind = MEND_UUID;
	else
		kind = MEND_NAME;

	const mend_note *note = mend_new_note(id, kind, value);
	if (!note) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	printf("%s\n", mend_note_uid(note));
	mend_free_note(note);
	return 0;
}
