#include <stdio.h>
#include <string.h>

#include "command/command.h"

int main(int argc, char *argv[])
{
	options opts;
	parse_options(&opts, argc, argv);
	char *cmd = opts.identifiers[0];
	if (strcmp(cmd, "le") == 0 || strcmp(cmd, "list-entities") == 0)
		list_entities(&opts);
	return 0;
}
