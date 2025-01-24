// Helper functions for imcsh.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h> // For wait() and waitpid()


// tokenize the input
char *tokens[100];
int tokenize(char *input, char *tokens[]) {
    int count = 0;

    // Split input into tokens by space
    char *token = strtok(input, " "); 
    
    while (token != NULL) { // While there are tokens
        tokens[count++] = token; // Add token to tokens array
        token = strtok(NULL, " "); // Get next token
    }
    
    // Add NULL to the end to indicate end of tokens
    tokens[count] = NULL; 
    return count;
}



void execute_command(char *tokens[], int is_background) { //is_background is a flag to indicate if the process should run in the background
    pid_t pid = fork(); // Create a child process

    if (pid < 0) {
        fprintf(stderr, "Fork failed"); 
        return 1;
    }

    if (pid == 0) { 
        // Child process
        if (is_background) {
            fclose(stdin); // Close standard input for background processes
        }

        if (execvp(tokens[0], tokens) == -1) { // Execute command with arguments in tokens
            fprintf("Error executing command");
            exit(EXIT_FAILURE); // Exit if command execution fails
        }
    } else {
        // Parent process
        if (is_background) {
            // Print PID of background process and continue
            printf("PID of background process: %d\n", pid);        
         
         } else {
            // Wait for the child process to finish
            waitpid(pid, NULL, 0); 
            printf("PID of finished process: %d\n", pid);
        }
    }   
}

void global_usage() {
    printf("IMCSH Version 1.1 created by Selin Meseli, Lisa-Maria Hollaus\n");
}


