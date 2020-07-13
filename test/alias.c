#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mend.h"

int alias_eq(const mend_alias *alias, const char *uid, const char *entity, const char *value, time_t since) {
	int ret = 0;
	if (strcmp(mend_alias_uid(alias), uid) != 0) {
		printf("uid %s failed to match\n", uid);
		ret = 1;
	}

	if (strcmp(mend_alias_entity_uid(alias), entity) != 0) {
		printf("entity %s failed to match\n", entity);
		ret = 1;
	}

	if (strcmp(mend_alias_value(alias), value) != 0) {
		printf("value %s failed to match\n", value);
		ret = 1;
	}
	
	if (mend_alias_since(alias) != since) {
		printf("since %ld failed to match\n", since);
		ret = 1;
	}

	return ret;
}

int mend_new_alias_test() {
	const mend_alias *alias = mend_new_alias("Foo", MEND_NAME, "Test alias");
	if (!alias) {
		printf("%s\n", mend_error());
		return 1;
	}

	if (strcmp(mend_alias_value(alias), "Test alias") != 0) {
		printf("name does not match\n");
		return 1;
	}

	return 0;
}

int mend_get_alias_by_uuid_test() {
	const mend_alias *alias = mend_get_alias("dd4e8f06-0124-4945-a58f-5d4931263799", MEND_UUID);
	if (!alias) {
		printf("%s\n", mend_error());
		return 1;
	}
	
	return alias_eq(alias,
			"dd4e8f06-0124-4945-a58f-5d4931263799",
			"a09d03f1-9e17-4127-b22c-08fbe65ef07f",
			"Foo alias",
			1006988400);
}

int mend_get_alias_by_value_test() {
	const mend_alias *alias = mend_get_alias("Foo alias", MEND_NAME);
	if (!alias) {
		printf("%s\n", mend_error());
		return 1;
	}

	return alias_eq(alias,
			"dd4e8f06-0124-4945-a58f-5d4931263799",
			"a09d03f1-9e17-4127-b22c-08fbe65ef07f",
			"Foo alias",
			1006988400);
}

int mend_remove_alias_by_uuid_test() {
	if (mend_remove_alias("3fcd6a93-0a4c-4241-ac56-3be36ca02ef8", MEND_UUID)) {
		printf("%s\n", mend_error());
		return 1;
	}
	return 0;
}

int mend_remove_alias_by_value_test() {
	if (mend_remove_alias("to_be_removed_by_value", MEND_NAME)) {
		printf("%s\n", mend_error());
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
	if (strcmp(cmd, "mend_new_alias") == 0)
		return mend_new_alias_test();
	else if (strcmp(cmd, "mend_get_alias_by_uuid") == 0)
		return mend_get_alias_by_uuid_test();
	else if (strcmp(cmd, "mend_get_alias_by_value") == 0)
		return mend_get_alias_by_value_test();
	else if (strcmp(cmd, "mend_remove_alias_by_uuid") == 0)
		return mend_remove_alias_by_uuid_test();
	else if (strcmp(cmd, "mend_remove_alias_by_value") == 0)
		return mend_remove_alias_by_value_test();
	printf("bad command (somebody wrote a test wrong)\n");
	return 1;
}
