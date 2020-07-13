#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "options.h"

int parse_options(options *options, int argc, char *argv[]) {
	int option;
	while ((option = getopt(argc, argv, ":hl")) != -1) {
		switch (option) {
			case 'h':
				fprintf(stderr, "opening manpage\n");
				system("man mend");
				exit(0);
			case 'l':
				options->long_format = 1;
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
