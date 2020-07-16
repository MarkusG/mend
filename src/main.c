#include <stdlib.h>
#include <string.h>

#include "../include/mend.h"
#include "command/command.h"
#include "options.h"
#include "utils.h"

int main(int argc, char *argv[])
{
	options opts = { 0 };
	parse_options(&opts, argc, argv);

	if (mend_init("dbname=mend")) {
		fprintf(stderr, ERR "%s\n", mend_error());
		return 1;
	}

	char *cmd = opts.identifiers[0];
	int exit_code;
	if (!cmd) {
		fprintf(stderr, ERR "no command specified. try mend -h for more information\n");
		exit_code = 1;
	} else if (strcmp(cmd, "le") == 0 || strcmp(cmd, "list-entities") == 0)
		exit_code = list_entities(&opts);
	else if (strcmp(cmd, "ne") == 0 || strcmp(cmd, "new-entity") == 0)
		exit_code = new_entity(&opts);
	else if (strcmp(cmd, "re") == 0 || strcmp(cmd, "rm-entity") == 0)
		exit_code = remove_entity(&opts);
	else if (strcmp(cmd, "na") == 0 || strcmp(cmd, "new-alias") == 0)
		exit_code = new_alias(&opts);
	else if (strcmp(cmd, "la") == 0 || strcmp(cmd, "list-aliases") == 0)
		exit_code = list_aliases(&opts);
	else if (strcmp(cmd, "ra") == 0 || strcmp(cmd, "rm-alias") == 0)
		exit_code = remove_alias(&opts);
	else if (strcmp(cmd, "nn") == 0 || strcmp(cmd, "new-note") == 0)
		exit_code = new_note(&opts);
	else if (strcmp(cmd, "ln") == 0 || strcmp(cmd, "list-notes") == 0)
		exit_code = list_notes(&opts);
	else if (strcmp(cmd, "en") == 0 || strcmp(cmd, "edit-note") == 0)
		exit_code = edit_note(&opts);
	else if (strcmp(cmd, "rn") == 0 || strcmp(cmd, "rm-note") == 0)
		exit_code = remove_note(&opts);
	else if (strcmp(cmd, "nr") == 0 || strcmp(cmd, "new-relation") == 0)
		exit_code = new_relation(&opts);
	else if (strcmp(cmd, "lr") == 0 || strcmp(cmd, "list-related") == 0)
		exit_code = list_related(&opts);
	else if (strcmp(cmd, "er") == 0 || strcmp(cmd, "edit-relation") == 0)
		exit_code = edit_relation(&opts);
	else if (strcmp(cmd, "rr") == 0 || strcmp(cmd, "rm-relation") == 0)
		exit_code = remove_relation(&opts);
	else if (strcmp(cmd, "s") == 0 || strcmp(cmd, "summarize") == 0)
		exit_code = summarize(&opts);

	mend_cleanup();
	free(opts.identifiers);
	return exit_code;
}
