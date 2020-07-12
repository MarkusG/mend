#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mend.h"

int mend_init_test() {
	if (mend_init(getenv("MEND_CONNECTION_STRING"))) {
		printf("%s\n", mend_error());
		return 1;
	}

	return 0;
}

int mend_cleanup_test() {
	mend_init(getenv("MEND_CONNECTION_STRING"));
	return mend_cleanup();
}

int main(int argc, char *argv[])
{
	const char *cmd = argv[1];
	if (strcmp(cmd, "mend_init") == 0)
		return mend_init_test();
	else if (strcmp(cmd, "mend_cleanup") == 0)
		return mend_cleanup_test();
	else {
		printf("no command specified (somebody wrote a test wrong)\n");
		return 1;
	}
}
