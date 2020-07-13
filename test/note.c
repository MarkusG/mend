#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mend.h"

int note_eq(
		const mend_note *note,
		const char *uid,
		const char *entity,
		const char *value,
		time_t created,
		time_t updated) {
	int ret = 0;
	if (strcmp(mend_note_uid(note), uid) != 0) {
		printf("uid %s failed to match\n", uid);
		ret = 1;
	}

	if (strcmp(mend_note_entity_uid(note), entity) != 0) {
		printf("entity %s failed to match\n", entity);
		ret = 1;
	}

	if (strcmp(mend_note_value(note), value) != 0) {
		printf("value %s failed to match\n", value);
		ret = 1;
	}
	
	if (mend_note_created(note) != created) {
		printf("created %ld failed to match\n", created);
		ret = 1;
	}

	if (updated != 0 && mend_note_updated(note) != updated) {
		printf("updated %ld failed to match\n", updated);
		ret = 1;
	}

	return ret;
}

int mend_new_note_test() {
	const mend_note *note = mend_new_note("Foo", MEND_NAME, "Test note");
	if (!note) {
		printf("%s\n", mend_error());
		return 1;
	}

	if (strcmp(mend_note_value(note), "Test note") != 0) {
		printf("value does not match\n");
		return 1;
	}

	return 0;
}

int mend_get_note_test() {
	const mend_note *note = mend_get_note("01a04e4b-0a7e-4aae-ba19-0a5500c35b66");
	if (!note) {
		printf("%s\n", mend_error());
		return 1;
	}
	
	return note_eq(note,
			"01a04e4b-0a7e-4aae-ba19-0a5500c35b66",
			"a09d03f1-9e17-4127-b22c-08fbe65ef07f",
			"Foo note",
			1004310000,
			1006988400);
}

int mend_edit_note_test() {
	const mend_note *edited = mend_edit_note("5f6d52a9-20f6-4dad-aa29-76ee6e097622", "New value");
	if (!edited) {
		printf("%s\n", mend_error());
		return 1;
	}

	return note_eq(edited,
			"5f6d52a9-20f6-4dad-aa29-76ee6e097622",
			"a09d03f1-9e17-4127-b22c-08fbe65ef07f",
			"New value",
			1004310000,
			0);
}

int mend_remove_note_test() {
	if (mend_remove_note("c97bc7a8-7a3a-4038-ad5f-11bfc875b8d7")) {
		printf("%s\n", mend_error());
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	if (mend_init(getenv("MEND_CONNECTION_STRING"))) {
		printf("%s\n", mend_error());
		return 1;
	}

	const char *cmd = argv[1];
	if (strcmp(cmd, "mend_new_note") == 0)
		return mend_new_note_test();
	else if (strcmp(cmd, "mend_get_note") == 0)
		return mend_get_note_test();
	else if (strcmp(cmd, "mend_edit_note") == 0)
		return mend_edit_note_test();
	else if (strcmp(cmd, "mend_remove_note") == 0)
		return mend_remove_note_test();
	printf("bad command (somebody wrote a test wrong)\n");
	return 1;
}
