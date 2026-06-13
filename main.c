#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 101
#define MAX_ARGS MAX_LINE
#define MAX_CMDS MAX_LINE

typedef struct {
    char* argv[MAX_ARGS];
    char* input_file;
    char* output_file;
} Command;

void test_output(const char* cmd) {
    fprintf(stdout, "Processed Input: \"%s\"\n", cmd);
}

void parse_and_run_command(const char* command) {
    char tokens[MAX_LINE][MAX_LINE];
    size_t token_count = 0;

    char token[MAX_LINE];    
    size_t token_len = 0; 
    size_t command_len = strlen(command);	    
     
    /*  Parse through command input
        Tokenize each command
        Append command to execution buffer*/
    for (size_t pos = 0; pos < command_len; pos++) {
       
        char c = command[pos];

        if (c == '\0' || c == ' ' || c == '\t' || c == '\v') {

            if (token_len > 0) {
                token[token_len] = '\0';

                strcpy(tokens[token_count], token);
                token_count++;

                token_len = 0;
            }

            if (c == '\0') break;

        } else {
            token[token_len++] = c;
        }
    }
    if (token_count == 0) {
        fprintf(stderr, "Invalid command\n");
        return;
    }
    
    // Parse commands
    Command cmds[MAX_CMDS];
    memset(cmds, 0, sizeof(cmds));

    size_t cmd_count = 1;
    size_t arg_index = 0;

    for (size_t t = 0; t < token_count; t++) {

        if (strcmp(tokens[t], "|") == 0) {

            if (arg_index == 0) {
                fprintf(stderr, "Invalid command\n");
                return;
            }

            cmds[cmd_count - 1].argv[arg_index] = NULL;
            cmd_count++;
            arg_index = 0;

        } else if (strcmp(tokens[t], "<") == 0) {

            if (t + 1 >= token_count) {
                fprintf(stderr, "Invalid command\n");
                return;
            }

            cmds[cmd_count - 1].input_file = tokens[++t];

        } else if (strcmp(tokens[t], ">") == 0) {

            if (t + 1 >= token_count) {
                fprintf(stderr, "Invalid command\n");
                return;
            }

            cmds[cmd_count - 1].output_file = tokens[++t];

        } else {
            cmds[cmd_count - 1].argv[arg_index++] = tokens[t];
        }
    }
    cmds[cmd_count - 1].argv[arg_index] = NULL;

    if (cmds[0].argv[0] == NULL) {
        fprintf(stderr, "Invalid command\n");
        return;
    }
    
    // Single command execution 
    if (cmd_count == 1) {
            
            pid_t pid = fork();

            if (pid < 0) {
                perror("fork");
                return;
            }

            if (pid == 0) {

                // '<' operator contingency
                if (cmds[0].input_file != NULL) {

                    int fd = open(cmds[0].input_file, O_RDONLY);

                    if (fd < 0) {
                        perror("open");
                        exit(1);
                    }

                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }

                // '>' operator contingency
                if (cmds[0].output_file != NULL) {

                    int fd = open(
                        cmds[0].output_file,
                        O_WRONLY | O_CREAT | O_TRUNC,
                        0666
                    );

                    if (fd < 0) {
                        perror("open");
                        exit(1);
                    }

                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                execv(cmds[0].argv[0], cmds[0].argv);

                perror("execv");
                exit(1);
            }

            // Wait for child process to finish executing
            int status;
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                printf("Exit status: %d\n", WEXITSTATUS(status));
            }

            return;
        }
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
