#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

// TODO open the user's editor to create the note
int edit_note(options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		exit(1);
	} else if (!is_uuid(id)) {
		fprintf(stderr, ERR "argument must be a UUID\n");
		exit(1);
	}

	const char *value = options->identifiers[2];
	if (!value) {
		fprintf(stderr, ERR "no value specified\n");
		exit(1);
	}

	const mend_note *note = mend_edit_note(id, value);
	if (!note) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	printf("%s\n", mend_note_uid(note));
	mend_free_note(note);
	return 0;
}
