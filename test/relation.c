#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/mend.h"

int relation_eq(
		const mend_relation *relation,
		const char *uid,
		const char *entity_a,
		const char *entity_b,
		const char *note,
		time_t created,
		time_t updated) {
	int ret = 0;
	if (strcmp(mend_relation_uid(relation), uid) != 0) {
		printf("uid %s failed to match\n", uid);
		ret = 1;
	}

	if (strcmp(mend_relation_entity_a_uid(relation), entity_a) != 0) {
		printf("entity_a %s failed to match\n", entity_a);
		ret = 1;
	}

	if (strcmp(mend_relation_entity_b_uid(relation), entity_b) != 0) {
		printf("entity_b %s failed to match\n", entity_b);
		ret = 1;
	}

	if (strcmp(mend_relation_note(relation), note) != 0) {
		printf("note %s failed to match\n", note);
		ret = 1;
	}
	
	if (mend_relation_created(relation) != created) {
		printf("created %ld failed to match\n", created);
		ret = 1;
	}

	if (updated != 0 && mend_relation_updated(relation) != updated) {
		printf("updated %ld failed to match\n", updated);
		ret = 1;
	}

	return ret;
}

int mend_new_relation_test() {
	const char *uids[] = {
		"a09d03f1-9e17-4127-b22c-08fbe65ef07f",
		"bc05cd9e-098a-42c6-a92f-21f4529a9f89"
	};

	mend_id_kind kinds[] = {
		MEND_UUID,
		MEND_UUID
	};

	const mend_relation *relation = mend_new_relation(uids, kinds, "Test relation");
	if (!relation) {
		printf("%s\n", mend_error());
		return 1;
	}

	if (strcmp(mend_relation_note(relation), "Test relation") != 0) {
		printf("name does not match\n");
		return 1;
	}

	return 0;
}

int mend_get_relation_test() {
	const mend_relation *relation = mend_get_relation("aea219b6-a39a-48f1-88f1-8c2affc528dd");
	if (!relation) {
		printf("%s\n", mend_error());
		return 1;
	}
	
	return relation_eq(relation,
			"aea219b6-a39a-48f1-88f1-8c2affc528dd",
			"a09d03f1-9e17-4127-b22c-08fbe65ef07f",
			"bc05cd9e-098a-42c6-a92f-21f4529a9f89",
			"Foo to bar",
			1001718000,
			1004310000);
}

int mend_edit_relation_test() {
	const mend_relation *edited = mend_edit_relation("027bf4d5-fc5a-480e-adfd-daabefe8dc28", "New value");
	if (!edited) {
		printf("%s\n", mend_error());
		return 1;
	}

	return relation_eq(edited,
			"027bf4d5-fc5a-480e-adfd-daabefe8dc28",
			"a09d03f1-9e17-4127-b22c-08fbe65ef07f",
			"bc05cd9e-098a-42c6-a92f-21f4529a9f89",
			"New value",
			1001718000,
			0);
}

int mend_remove_relation_test() {
	if (mend_remove_relation("979fdd4d-4142-40e0-88e6-ae8cd2987f5a")) {
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
	if (strcmp(cmd, "mend_new_relation") == 0)
		return mend_new_relation_test();
	else if (strcmp(cmd, "mend_get_relation") == 0)
		return mend_get_relation_test();
	else if (strcmp(cmd, "mend_edit_relation") == 0)
		return mend_edit_relation_test();
	else if (strcmp(cmd, "mend_remove_relation") == 0)
		return mend_remove_relation_test();
	printf("bad command (somebody wrote a test wrong)\n");
	return 1;
}
