#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys/types.h>

ssize_t lineread(int fd, char *buf, size_t sz);
char *dynamic_lineread(int fd);
char *next_token(char **str_ptr, const char *delim);

#endif