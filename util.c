#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <math.h>

#include "util.h"
#include "logger.h"

ssize_t lineread(int fd, char *buf, size_t sz)
{
    size_t total_read = 0;
    while (total_read < sz) {
        char c;
        ssize_t read_sz = read(fd, &c, 1);
        if (read_sz == 0) {
            return total_read;
        } else if (read_sz == -1) {
            return -1;
        }

        buf[total_read] = c;
        total_read += read_sz;
        if (c == '\n') {
            return total_read;
        }
    }
    return total_read;
}

char *dynamic_lineread(int fd)
{
    size_t buf_sz = 64;
    char *buf = NULL;
    size_t read_total = 0;

    while (true) {
        char *tmp_buf = realloc(buf, buf_sz);
        if (tmp_buf == NULL) {
            free(buf);
            return NULL;
        }

        buf = tmp_buf;

        ssize_t read_sz = lineread(fd, buf + read_total, buf_sz - read_total);

        if (read_sz == 0 || read_sz == -1) {
            free(buf);
            return NULL;
        }

        read_total += read_sz;
        size_t last_character = read_total - 1;
        if (buf[last_character] == '\n' || read_total < buf_sz) {
            buf[last_character] = '\0';
            return buf;
        }

        buf_sz = buf_sz * 2;
    }

    return NULL;
}

char *next_token(char **str_ptr, const char *delim)
{
    if (*str_ptr == NULL) {
        return NULL;
    }

    size_t tok_start = strspn(*str_ptr, delim);
    size_t tok_end = strcspn(*str_ptr + tok_start, delim);

    /* Zero length token. We must be finished. */
    if (tok_end  == 0) {
        *str_ptr = NULL;
        return NULL;
    }

    /* Take note of the start of the current token. We'll return it later. */
    char *current_ptr = *str_ptr + tok_start;

    /* Shift pointer forward (to the end of the current token) */
    *str_ptr += tok_start + tok_end;

    if (**str_ptr == '\0') {
        /* If the end of the current token is also the end of the string, we
         * must be at the last token. */
        *str_ptr = NULL;
    } else {
        /* Replace the matching delimiter with a NUL character to terminate the
         * token string. */
        **str_ptr = '\0';

        /* Shift forward one character over the newly-placed NUL so that
         * next_pointer now points at the first character of the next token. */
        (*str_ptr)++;
    }

    return current_ptr;
}