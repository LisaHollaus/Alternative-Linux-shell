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
    return count; // Return number of tokens
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
            fprintf(stderr, "Error executing command");
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


void execute_command_with_redirection(char *tokens[], const char *output_file) {
    pid_t pid = fork();  // Create a child process

    if (pid < 0) {
        fprintf(stderr, "Fork failed"); 
        return 1;

    } else if (pid == 0) {

        // Child process
        printf("Child: Redirecting output to %s...\n", output_file);

        // Open the file in append mode
        FILE *file = fopen(output_file, "a");  // Open in append mode
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        // Redirect stdout to the file
        int file_fd = fileno(file);  // Get the file descriptor from FILE*
        dup2(file_fd, STDOUT_FILENO);  // Redirect stdout to the file (in append mode)
        fclose(file);  // Close FILE* (descriptor stays open because dup2 duplicated it)

        // Execute the command 
        if (execvp(tokens[0], tokens) == -1) {
            perror("Error executing command");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        wait(NULL);  // Wait for the child process to finish
        printf("Parent: Child process completed. PID was %d\n", pid);
    }
}

void quite_programm() {
    // The following processes are running, are you sure you want to quit? [Y/n]. 
    //A list of all currently running processes will follow. If the user enters Y, the shell will quit and all running processes will be terminated.
    printf("Exiting IMCSH...\n");
    exit(0);
}