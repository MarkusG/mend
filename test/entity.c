#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mend.h"

int entity_eq(
		const mend_entity *entity,
		const char *uid,
		const char *name,
		const time_t created) {
	int ret = 0;
	if (strcmp(mend_entity_uid(entity), uid) != 0) {
		printf("uid %s failed to match\n", uid);
		ret = 1;
	}

	if (strcmp(mend_entity_name(entity), name) != 0) {
		printf("name %s failed to match\n", name);
		ret = 1;
	}
	
	if (mend_entity_created(entity) != created) {
		printf("created %ld failed to match\n", created);
		ret = 1;
	}

	return ret;
}

int mend_new_entity_test() {
	const mend_entity *entity = mend_new_entity("Test entity");
	if (!entity) {
		printf("%s\n", mend_error());
		return 1;
	}

	if (strcmp(mend_entity_name(entity), "Test entity") != 0) {
		printf("name does not match\n");
		return 1;
	}

	return 0;
}

int mend_get_entities_test() {
	const mend_entity **entities = mend_get_entities();
	if (!entities) {
		printf("%s\n", mend_error());
		return 1;
	}

	int matched[3] = { 0, 0, 0 };
	for (int i = 0; i < 3; ++i) {
		if (!entity_eq(entities[i],
					   "a09d03f1-9e17-4127-b22c-08fbe65ef07f",
					   "Foo",
					   1001718000))
			matched[0] = 1;
		else if (!entity_eq(entities[i],
					   "bc05cd9e-098a-42c6-a92f-21f4529a9f89",
					   "Bar",
					   1004310000))
			matched[1] = 1;
		else if (!entity_eq(entities[i],
					   "17e3f02a-8e2d-4a66-abca-d1082a9f9508",
					   "Baz",
					   1006988400))
			matched[2] = 1;
	}
	
	for (int i = 0; i < 3; ++i) {
		if (!matched[i]) {
			switch (i) {
				case 0:
					printf("entity Foo failed to match");
					break;
				case 1:
					printf("entity Bar failed to match");
					break;
				case 2:
					printf("entity Baz failed to match");
					break;
			}
			return 1;
		}
	}

	return 0;
}

int mend_get_entity_by_name_test() {
	const mend_entity *entity = mend_get_entity("Foo", MEND_NAME);
	if (!entity) {
		printf("%s\n", mend_error());
		return 1;
	}

	return entity_eq(entity,
			"a09d03f1-9e17-4127-b22c-08fbe65ef07f",
			"Foo",
			1001718000);
}

int mend_get_entity_by_uuid_test() {
	const mend_entity *entity = mend_get_entity("a09d03f1-9e17-4127-b22c-08fbe65ef07f", MEND_UUID);
	if (!entity) {
		printf("%s\n", mend_error());
		return 1;
	}

	return entity_eq(entity,
			"a09d03f1-9e17-4127-b22c-08fbe65ef07f",
			"Foo",
			1001718000);
}

int mend_remove_entity_by_name_test() {
	if (mend_remove_entity("to_be_removed", MEND_NAME)) {
		printf("%s\n", mend_error());
		return 1;
	}
	return 0;
}

int mend_remove_entity_by_uuid_test() {
	if (mend_remove_entity("ddbfba06-b10c-4344-a343-ccc63321d435", MEND_UUID)) {
		printf("%s\n", mend_error());
		return 1;
	}
	return 0;
}

int mend_uid_from_name_test() {
	const char *uid = mend_uid_from_name("Foo");
	if (!uid) {
		printf("%s\n", mend_error());
		return 1;
	}

	if (strcmp(uid, "a09d03f1-9e17-4127-b22c-08fbe65ef07f") != 0) {
		printf("uid/name mismatch\n");
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	if (mend_init(getenv("MEND_CONNECTION_STRING"))) {
		printf("%s\n", mend_error());
		return 1;
	}

	const char *cmd = argv[1];
	if (strcmp(cmd, "mend_new_entity") == 0)
		return mend_new_entity_test();
	else if (strcmp(cmd, "mend_get_entities") == 0)
		return  mend_get_entities_test();
	else if (strcmp(cmd, "mend_get_entity_by_name") == 0)
		return  mend_get_entity_by_name_test();
	else if (strcmp(cmd, "mend_get_entity_by_uuid") == 0)
		return  mend_get_entity_by_uuid_test();
	else if (strcmp(cmd, "mend_remove_entity_by_name") == 0)
		return  mend_remove_entity_by_name_test();
	else if (strcmp(cmd, "mend_remove_entity_by_uuid") == 0)
		return  mend_remove_entity_by_uuid_test();
	else if (strcmp(cmd, "mend_uid_from_name") == 0)
		return  mend_uid_from_name_test();
	printf("bad command (somebody wrote a test wrong)\n");
	return 1;
}
