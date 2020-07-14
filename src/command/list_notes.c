#include <stdlib.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int list_notes(options *options) {
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
	const mend_note **notes = mend_get_notes(id, kind);
	if (!notes) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}
	print_entity(entity, options);
	int i = 0;
	const mend_note *note;
	while ((note = notes[i])) {
		printf("\n%s:\n%s\n", trunc_uuid(mend_note_uid(note)), mend_note_value(note));
		mend_free_note(note);
		++i;
	}
	free((void*)notes);
	mend_free_entity(entity);
	return 0;
}
