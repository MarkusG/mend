#include <stdlib.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

int summarize(options *options) {
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
	if (!entity) {
		printf("%s\n", mend_error());
		return 1;
	}
	char timebuf[20];
	time_t created = localize(mend_entity_created(entity));
	strftime(timebuf, sizeof(timebuf), "%F %R", localtime(&created));

	printf("uuid: %s\nname: %s\ncreated: %s\n",
			mend_entity_uid(entity),
			mend_entity_name(entity),
			timebuf);

	const mend_alias **aliases = mend_get_aliases(id, kind);
	if (aliases) {
		printf("\nalso known as:\n");

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
	}

	const mend_note **notes = mend_get_notes(id, kind);
	if (notes) {
		printf("\nnotes:");

		int i = 0;
		const mend_note *note;
		while ((note = notes[i])) {
			printf("\n%s:\n%s\n",
					mend_note_uid(note),
					mend_note_value(note));
			mend_free_note(note);
			++i;
		}
		free((void*)notes);
	}

	const mend_entity **related = mend_get_related_entities(id, kind);
	if (related) {
		printf("\nrelated to:\n");

		int i = 0;
		const mend_entity *rel_entity;
		while ((rel_entity = related[i])) {
			print_entity(rel_entity, options);
			mend_free_entity(rel_entity);
			++i;
		}
		free((void*)related);
	}

	return 0;
}
