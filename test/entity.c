#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mend.h"

int mend_new_entity_test() {

}

int mend_free_entity_test() {

}

int mend_get_entities_test() {

}

int mend_get_entity_test() {

}

int mend_remove_entity_test() {

}

int mend_uid_from_name_test() {

}

int mend_entity_uid_test() {

}

int mend_entity_name_test() {

}

int mend_entity_created_test() {

}

int main(int argc, char *argv[])
{
	if (mend_init(getenv("MEND_CONNECTION_STRING"))) {
		printf("%s\n", mend_error());
		return 1;
	}

	const char *cmd = argv[1];
	if (strcmp(cmd, "mend_new_entity") == 0)
		return mend_new_entity_test();
	else if (strcmp(cmd, "mend_free_entity") == 0)
		return  mend_free_entity_test();
	else if (strcmp(cmd, "mend_get_entities") == 0)
		return  mend_get_entities_test();
	else if (strcmp(cmd, "mend_get_entity") == 0)
		return  mend_get_entity_test();
	else if (strcmp(cmd, "mend_remove_entity") == 0)
		return  mend_remove_entity_test();
	else if (strcmp(cmd, "mend_uid_from_name") == 0)
		return  mend_uid_from_name_test();
	else if (strcmp(cmd, "mend_entity_uid") == 0)
		return  mend_entity_uid_test();
	else if (strcmp(cmd, "mend_entity_name") == 0)
		return  mend_entity_name_test();
	else if (strcmp(cmd, "mend_entity_created") == 0)
		return  mend_entity_created_test();
	printf("bad command (somebody wrote a test wrong)\n");
	return 1;
}
