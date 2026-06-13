#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 101
#define SPACE ' '

void test_output(const char* cmd) {
    fprintf(stdout, "Processed Input: %s\n", cmd);
}

void parse_and_run_command(const char* command) {
    char commands_to_execute[MAX_LINE][MAX_LINE];
    size_t command_count = 0;

    char token[MAX_LINE];    
    size_t token_len = 0, command_len = strlen(command);	    
     
    /*  Parse through command input
        Tokenize each command
        Append command to execution buffer*/
    for (size_t pos = 0; pos < command_len; pos++) {
       
        // Begin reading after parsing whitespace 
        if (command[pos] != SPACE) {
            
            // Write to token until next occurence of whitespace 
            while(command[pos] != SPACE && command[pos] != '\0') {
                token[token_len] = command[pos];
                token_len++;
                pos++;
            } 
            
            token[token_len] = '\0';

            // Add valid token to list of commands
            strcpy(commands_to_execute[command_count], token);
            command_count++;
        }
        
        // Reset token after command read
        memset(token, 0, MAX_LINE); 
        token_len = 0;
    }

    if (command_count > 0) {
        
        /*  Execute commands from buffer in sequential order
            
        */
        for (size_t it = 0; it < command_count; it++) {
            
            // Test if command is being read properly
            test_output(commands_to_execute[it]);
            
            if (strcmp(commands_to_execute[it], "exit") == 0) {
                
                exit(0);
            } else {
                fprintf(stderr, "Invalid command\n");
            }


        }
        
    } else {
        fprintf(stderr, "Invalid command\n");
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
