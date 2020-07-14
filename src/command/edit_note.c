#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "command.h"
#include "../utils.h"
#include "../../include/mend.h"

// TODO open the user's editor to create the note
int edit_note(options *options) {
	const char *id = options->identifiers[1];
	if (!id) {
		fprintf(stderr, ERR "no identifier specified\n");
		return 1;
	} else if (!is_uuid(id)) {
		fprintf(stderr, ERR "argument must be a UUID\n");
		return 1;
	}

	const mend_note *prev_note = mend_get_note(id);
	if (!prev_note) {
		fprintf(stderr, ERR "no such note %s\n", id);
		return 1;
	}
	const char *prev_value = mend_note_value(prev_note);
	FILE *note_f = fopen("/tmp/mend_note", "w");
	fputs(prev_value, note_f);
	mend_free_note(prev_note);
	fclose(note_f);

	const char *value;
	int edit_ret = edit_path("/tmp/mend_note", &value);
	if (edit_ret == 1)
		return 1;
	else if (edit_ret == 2) {
		fprintf(stderr, "new note is empty. do you want to delete it? (y/n) ");
		struct termios old, new;
		tcgetattr(STDIN_FILENO, &new);
		old = new;
		new.c_lflag &= ~ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &new);
		char response = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &old);
		putchar('\n');
		if (response == 'y') {
			fprintf(stderr, "deleting note %s\n", id);
			return mend_remove_note(id);
		} else {
			fprintf(stderr, "exiting\n");
			return 0;
		}

	}
	const mend_note *note = mend_edit_note(id, value);
	if (!note) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	printf("%s\n", mend_note_uid(note));
	mend_free_note(note);
	free((void*)value);
	return 0;
}
