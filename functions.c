// Helper functions for imcsh.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"


#include <signal.h> // For signal()

#include <sys/types.h>
#include <sys/wait.h> // For wait() and waitpid()

pid_t running_processes[100];  // Stores PIDs of background processes (max 100)
int process_count = 0;  // Number of active background processes

void add_process(pid_t pid) {
    if (process_count < 100) { // max 100 processes
        running_processes[process_count] = pid;
        process_count++;
    } else {
        fprintf(stderr, "Process limit reached. Cannot track more processes.\n");
    }
}

void remove_process(pid_t pid) {
    for (int i = 0; i < process_count; i++) {
        if (running_processes[i] == pid) {
            // Shift remaining processes
            for (int j = i; j < process_count - 1; j++) {
                running_processes[j] = running_processes[j + 1];
            }
            process_count--;
            break;
        }
    }
}

void print_running_processes() {
    if (process_count == 0) {
        printf("No background processes are running.\n");
        return;
    }
    printf("The following processes are running:\n");
    for (int i = 0; i < process_count; i++) {
        printf("PID: %d\n", running_processes[i]);
    }
}



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


// void for executing the command without a return value
void execute_command(char *tokens[], int is_background) { //is_background is a flag to indicate if the process should run in the background
    pid_t pid = fork(); // Create a child process

    if (pid < 0) {
        fprintf(stderr, "Fork failed"); 
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
            add_process(pid); // Add PID to running_processes array     
         
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

        // Check if the command is globalusage
        if (strcmp(tokens[0], "globalusage") == 0) {
            // Execute the internal command
            global_usage();
            exit(EXIT_SUCCESS);  // Exit the child process after execution
        }

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

void quit_program() {
    if (process_count > 0) {
        // Print running processes and ask for confirmation
        print_running_processes();
        printf("Are you sure you want to quit? [Y/n]\n");

        char response;
        scanf(" %c", &response);
        if (response == 'Y' || response == 'y') {
            // Terminate all running background processes
            for (int i = 0; i < process_count; i++) {
                kill(running_processes[i], SIGTERM); // Send termination signal
            }
            printf("All background processes terminated. Exiting IMCSH...\n");
            exit(0);
        } else {
            printf("Quit aborted.\n");
            return;
        }
    } else {
        printf("Exiting IMCSH...\n");
        exit(0);
    }
}