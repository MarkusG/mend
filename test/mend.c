#include <stdio.h>
#include <string.h>

#include "../include/mend.h"

int main(int argc, char *argv[])
{
	if (mend_init("postgresql://postgres@db/mend")) {
		printf("%s\n", mend_error());
		return 1;
	}

	if (strcmp(argv[1], "mend_cleanup"))
		return mend_cleanup();

	return 0;
}
