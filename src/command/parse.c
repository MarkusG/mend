#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

int parse_options(options *options, int argc, char *argv[]) {
	int option;
	while ((option = getopt(argc, argv, ":h")) != -1) {
		switch (option) {
			case 'h':
				// TODO actual help command this is just a placeholder
				fprintf(stderr, "help\n");
				/* exit(0); */
				break;
			case '?':
				fprintf(stderr, "Unknown option: %c\n", optopt);
				break;
		}
	}
	
	options->identifiers = (char**)malloc(argc * sizeof(char*));
	int i;
	for (i = 0; optind < argc; optind++, i++) {
		options->identifiers[i] = argv[optind];
	}
	options->identifiers[i] = NULL;
	return 0;
}
