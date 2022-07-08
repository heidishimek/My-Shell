#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include "leetify.h"

/* create a new pipe */
int fds[2];

void execute_pipeline(struct command_line *cmds)
{
    /* fork a new process */
    pid_t pid = fork();

    /* if pid is the child: */
    if (pid == 0) {
        /* dup2 stdout to pipe[1] */
        dup2(fds[1], STDOUT_FILENO);
        /* close pipe[0] */
        close(fds[0]);
        /* execvp the command */
        execvp(*cmds->tokens, cmds->tokens);

    /* if pid is the parent: */
    } else {
        /* dup2 stdin to pipe[0] */
        dup2(fds[0], STDIN_FILENO);
        /* close pipe[1] */
        close(fds[1]);
        /* move on to the next command in the pipeline */
        execute_pipeline(cmds + 1);
    }
}
