#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 101

void parse_and_run_command(const char *command) {
    /* TODO: Implement this.
       Note that this is not the correct way to test for the exit command.
       For example the command "  exit  " should also exit your shell. */
    if (strcmp(command, "exit") == 0) {
        exit(0);
    }
    fprintf(stderr, "Not implemented.\n");
}

int main(void) {
    char line[MAX_LINE];

    while (1) {
        printf("> ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;
        line[strcspn(line, "\n")] = '\0';
        parse_and_run_command(line);
    }

    return 0;
}
