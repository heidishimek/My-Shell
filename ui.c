#include <stdio.h>
#include <readline/readline.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>

#include "history.h"
#include "logger.h"
#include "ui.h"
#include "util.h"

static const char *good_str = "😌";
static const char *bad_str  = "🤯";
static bool scripting = false;
static int last_status = 0;

static int readline_init(void);

void init_ui(void)
{
    LOGP("Initializing UI...\n");

    char *locale = setlocale(LC_ALL, "en_US.UTF-8");
    LOG("Setting locale: %s\n",
            (locale != NULL) ? locale : "could not set locale!");

    rl_startup_hook = readline_init;

    if (isatty(STDIN_FILENO) == false) {
        scripting = true;
    }
}

void destroy_ui(void)
{
    // TODO cleanup code, if necessary
}

char *prompt_line(void)
{
    const char *status = prompt_status() ? bad_str : good_str;

    char cmd_num[25];
    snprintf(cmd_num, 25, "%ud", prompt_cmd_num());

    char *user = prompt_username();
    char *host = prompt_hostname();
    char *cwd = prompt_cwd();

    char *format_str = ">>-[%s]-[%s]-[%s@%s:%s]-> ";

    size_t prompt_sz
        = strlen(format_str)
        + strlen(status)
        + strlen(cmd_num)
        + strlen(user)
        + strlen(host)
        + strlen(cwd)
        + 1;

    char *prompt_str =  malloc(sizeof(char) * prompt_sz);

    snprintf(prompt_str, prompt_sz, format_str,
            status,
            cmd_num,
            user,
            host,
            cwd);

    return prompt_str;
}

char *prompt_username(void)
{
    struct passwd *pws = getpwuid(geteuid());
    if (pws) {
        return pws->pw_name;
    }
    return "unknown user";
}

char *prompt_hostname(void)
{
    static char host[HOST_NAME_MAX + 1];
    gethostname(host, HOST_NAME_MAX);
    return host;
}

char *prompt_cwd(void)
{
    static char cwd[256];
    getcwd(cwd, 256);
    char *home = getenv("HOME");

    char *new_cwd = cwd;
    if (strncmp(home, cwd, strlen(home)) == 0) {
        new_cwd = cwd + strlen(home) - 1;
        new_cwd[0] = '~';
    }

    return new_cwd;
}

void set_status(int status) {
    last_status = status;
}

int prompt_status(void)
{
    return last_status;
}

unsigned int prompt_cmd_num(void)
{
    return 0;
}

char *read_command(void)
{
    if (scripting == false) {
        char *prompt = prompt_line();
        char *command = readline(prompt);
        free(prompt);
        return command;
    } else {
        return NULL;
    }
}

int readline_init(void)
{
    rl_bind_keyseq("\\e[A", key_up);
    rl_bind_keyseq("\\e[B", key_down);
    rl_variable_bind("show-all-if-ambiguous", "on");
    rl_variable_bind("colored-completion-prefix", "on");
    rl_attempted_completion_function = command_completion;
    return 0;
}

int key_up(int count, int key)
{
    /* Modify the command entry text: */
    rl_replace_line("User pressed 'up' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: step back through the history until no more history entries are
    // left. Once the end of the history is reached, stop updating the command
    // line.

    return 0;
}

int key_down(int count, int key)
{
    /* Modify the command entry text: */
    rl_replace_line("User pressed 'down' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: step forward through the history (assuming we have stepped back
    // previously). Going past the most recent history command blanks out the
    // command line to allow the user to type a new command.

    return 0;
}

char **command_completion(const char *text, int start, int end)
{
    /* Tell readline that if we don't find a suitable completion, it should fall
     * back on its built-in filename completion. */
    rl_attempted_completion_over = 0;

    return rl_completion_matches(text, command_generator);
}

/**
 * This function is called repeatedly by the readline library to build a list of
 * possible completions. It returns one match per function call. Once there are
 * no more completions available, it returns NULL.
 */
char *command_generator(const char *text, int state)
{
    // TODO: find potential matching completions for 'text.' If you need to
    // initialize any data structures, state will be set to '0' the first time
    // this function is called. You will likely need to maintain static/global
    // variables to track where you are in the search so that you don't start
    // over from the beginning.

    return NULL;
}