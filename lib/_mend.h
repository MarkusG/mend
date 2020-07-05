#ifndef ___MEND_H__
#define ___MEND_H__
#include <libpq-fe.h>
extern PGconn *_conn;
extern char *_error;
void _set_error(const char*, ...);
#endif /* ifndef ___MEND_H__ */
