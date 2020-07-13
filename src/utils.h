#ifndef __MEND_UTILS_H__
#define __MEND_UTILS_H__
#define WARN "\x1b[33mwarn: \x1b[0m"
#define ERR "\x1b[31merror: \x1b[0m"
int is_uuid(const char*);
const char *trunc_uuid(const char*);
time_t localize(time_t);
#endif /* ifndef __MEND_UTILS_H__ */
