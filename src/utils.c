#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"

int is_uuid(const char *input) {
	unsigned int a, b, c, d, e;
	int result = sscanf(input, "%08x-%04x-%04x-%04x-%012x", &a, &b, &c, &d, &e);
	return result == 5;
}

const char *trunc_uuid(const char *uuid) {
	char *ret = strdup(uuid);
	ret[8] = '\0';
	return (const char*)ret;
}

time_t localize(time_t utc) {
	const time_t now = time(NULL);
	struct tm now_bd = { 0 };
	localtime_r(&now, &now_bd);

	return utc - now_bd.tm_gmtoff;
}

void print_entity(const mend_entity *entity, options *options) {
	if (!options->long_format) {
		const char *uid_trunc = trunc_uuid(mend_entity_uid(entity));
		printf("%s %s\n",
				uid_trunc,
				mend_entity_name(entity));
		free((void*)uid_trunc);
	} else {
		char timebuf[20];
		const time_t created = localize(mend_entity_created(entity));
		strftime(timebuf, sizeof(timebuf), "%F %R", localtime(&created));

		printf("%s %s %s\n",
				timebuf,
				mend_entity_uid(entity),
				mend_entity_name(entity));
	}
}

int edit_path(const char *path, const char **out) {
	FILE *note_f = fopen(path, "a+");
	if (note_f == NULL) {
		fprintf(stderr, ERR "could not open %s: %s\n",
				path,
				strerror(errno));
		*out = NULL;
		return 1;
	}
	const char *editor = getenv("EDITOR");
	if (!editor) {
		fprintf(stderr, WARN "EDITOR not set. using vi\n");
		editor = "vi";
	}
	char command[64];
	sprintf(command, "%s %s", editor, path);
	system(command);

	struct stat path_st;
	stat(path, &path_st);
	int size = path_st.st_size;
	if (size == 0) {
		fclose(note_f);
		remove(path);
		*out = NULL;
		return 2;
	}

	char *note_s = malloc((size + 1) * sizeof(char));
	char c;
	int i = 0;
	while ((c = fgetc(note_f)) != EOF) {
		note_s[i++] = c;
	}
	note_s[i] = '\0';
	fclose(note_f);

	remove(path);
	*out = (const char*)note_s;
	return 0;
}
