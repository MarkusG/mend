#ifndef __MEND_OPTIONS_H__
#define __MEND_OPTIONS_H__
typedef struct options {
	char **identifiers;
	char long_format;
} options;

int parse_options(options*, int, char*[]);
#endif /* ifndef __MEND_OPTIONS_H__ */
