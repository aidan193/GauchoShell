#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 101
#define SPACE ' '

void parse_and_run_command(const char *command) {
    /* TODO: Implement this.
       Note that this is not the correct way to test for the exit command.
       For example the command "  exit  " should also exit your shell. */	
    char token [MAX_LINE];    
    size_t token_len = 0, command_len = strlen(command);	    
    
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
 
            if (strcmp(token, "exit") == 0) {
                exit(0);
            }
             
            fprintf(stderr, "Not implemented.\n");
        }
        
        memset(token, 0, MAX_LINE); 
        token_len = 0;
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
