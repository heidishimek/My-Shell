/**
 * @file
 *
 * Text-based UI functionality. These functions are primarily concerned with
 * interacting with the readline library.
 */

#ifndef _UI_H_
#define _UI_H_

#include <sys/types.h>

void init_ui(void);

char *prompt_line(void);
char *prompt_username(void);
char *prompt_hostname(void);
char *prompt_cwd(void);
void set_status(int stauts);
int prompt_status(void);
unsigned int prompt_cmd_num(void);

char *read_command(void);

int key_up(int count, int key);
int key_down(int count, int key);

char **command_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);

ssize_t lineread(int fd, char *buf, size_t sz);
char *dynamic_lineread(int fd);
char *next_token(char **str_ptr, const char *delim);

#endif
