/**
 * @file
 *
 * Contains shell history data structures and retrieval functions.
 */

#ifndef _LEETIFY_H
#define _LEETIFY_H

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

struct command_line {
    char **tokens;
    bool stdout_pipe;
    char *stdout_file;
};

void execute_pipeline(struct command_line *cmds);

#endif
