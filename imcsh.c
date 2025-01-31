#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main() {
    // set up variables
    char input[1024];
    char *tokens[100];

    while (1) {
        printf("user@host> ");

        // Read input from stdin
        // fgets(input, 1024, stdin); 
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit loop if no input
        }
        
        // Remove newline character from input
        input[strcspn(input, "\n")] = 0;
        
        // Tokenize input
        int count = tokenize(input, tokens);  

        
        // check if ">" is present
        if (strstr(input, ">") != NULL) { 
            char *command = strtok(input, ">"); // everything before ">"
            char *filename = strtok(NULL, ">"); // everything after ">"
            
            // Trim whitespace
            filename[strcspn(filename, "\n")] = 0; // Remove newline character
            while (*filename == ' ') {
                filename++; // Remove leading whitespace
            }
            
            // Tokenize command
            tokenize(command, tokens);
            // Execute command with redirection
            execute_command_with_redirection(tokens, filename);
            continue;
        }
        
        // Execute command based on first token

        if (strncmp(tokens[0], "exec", 4) == 0) {
            int is_background = 0; // Flag to indicate if process should run in background
            
            // Check for & at the end
            if (count > 2 && strcmp(tokens[count - 1], "&") == 0) {
                is_background = 1; // Set flag if "&" is present
                tokens[count - 1] = NULL; // Remove the & from tokens
            }

            // Remove the "exec" part and pass the rest to execute_command
            execute_command(tokens + 1, is_background);
            continue;
        }
        
        // Check for globalusage command
        if (strcmp(tokens[0], "globalusage") == 0) {
            global_usage();
            continue;
        } 

        // Check for quit command
        if (strcmp(tokens[0], "quit") == 0) {
            quit_programm();
            break;
        } 

        // If no special command is found
        printf("Command not found\n");
    }
}




