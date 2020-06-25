#include <stdio.h>
#include <stdlib.h>

#include <libpq-fe.h>

#include "../include/mend.h"

int main(int argc, char *argv[])
{
	mend_init("dbname=mend");
	entity **mark_entities = get_entities_from_alias("Mark");
	int i = 0;
	while (mark_entities[i]) {
		printf("%s\n", entity_uuid(mark_entities[i]));
		free_entity(mark_entities[i]);
		++i;
	}
	free(mark_entities);
	mend_cleanup();
	return 0;
}
