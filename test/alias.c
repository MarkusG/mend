#include <stdio.h>
#include <string.h>

#include "../include/mend.h"

int pass1() {
	return 0;
}

int pass2() {
	return 0;
}

int fail() {
	return 0;
}

int main(int argc, char *argv[])
{
	if (mend_init("postgresql://postgres@db/mend")) {
		printf("%s\n", mend_error());
		return 1;
	}

	if (strcmp(argv[1], "pass1") == 0)
		return pass1();
	else if (strcmp(argv[1], "pass2") == 0)
		return pass2();
	else if (strcmp(argv[1], "fail") == 0)
		return fail();
	else
		return 1;
}
