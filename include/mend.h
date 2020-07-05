#ifndef __MEND_H__
#define __MEND_H__
typedef enum mend_id_kind {
	MEND_UUID,
	MEND_NAME
} mend_id_kind;

int mend_init(const char*);
const char *mend_error();
int mend_cleanup();
int mend_new_entity(const char*, const char**);
int mend_remove_entity(const char*, mend_id_kind);
int mend_uid_from_name(const char*, const char**);
#endif /* ifndef __MEND_H__ */
