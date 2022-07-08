#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <readline/readline.h>

#include "history.h"
#include "logger.h"
#include "ui.h"
#include "util.h"
#include "leetify.h"

int cd_handler(char *args[]) {
    if (args[1] == NULL) {
        args[1] = getenv("HOME");
        chdir(args[1]);
    } else if (chdir(args[1]) != 0) {
        perror("no dir");
    }
    return 0;
}

int exit_handler(char *args[]) {
    exit(EXIT_SUCCESS);
    return 0;
}

void sigint_handler(int sig) {
    if (isatty(STDIN_FILENO)) {
        printf("\n");
        fflush(stdout);
    }
}

struct builtin {
    char name[25];
    int (*function)(char *args[]);
};

struct builtin builtins[] = {
    { "exit", exit_handler },
    { "cd", cd_handler },
};

int handle_builtins(char *args[]) {
    if (args[0] == NULL) {
        return -1;
    }

    for (int i = 0; i < sizeof(builtins) / sizeof(struct builtin); ++i) {
        if (strncmp(builtins[i].name, args[0], strlen(builtins[i].name)) == 0) {
            return builtins[i].function(args);
        }
    }

    return -1; // arg[0] was not a builtin
}

int main(int argc, char *argv[]) {

    signal(SIGINT, sigint_handler);

    while (true) {

        char *line = dynamic_lineread(fileno(stdin));

        if (line == NULL) {
            break;
        }

        char *tokens[4096];
        int token_count = 0;
        char *next_tok = line;
        char *curr_tok;

        while ((curr_tok = next_token(&next_tok, " \t\r\n")) != NULL) {
            if (*curr_tok == '#') {
                break;
            } 
            tokens[token_count++] = curr_tok;
        }

        tokens[token_count] = (char *) 0;

        if (handle_builtins(tokens) == 0) {
            goto cleanup;
        }

        pid_t child = fork();
        if (child == -1) {
            perror("fork");
        } else if (child == 0) {
            if (execvp(tokens[0], tokens) == -1) {
                perror("exec");
                close(fileno(stdin));
                close(fileno(stdout));
                close(fileno(stderr));
                exit(EXIT_FAILURE);
            }
        } else {
            int status;
            wait(&status);
            set_status(status);
        }

cleanup:
        free(line);
    }

    return 0;
}