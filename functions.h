// to avoide multiple inclusion of header files
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sys/types.h>

// Function prototypes for functions.c
int tokenize(char *input, char *tokens[]);
void execute_command(char *tokens[], int is_background);
void execute_command_with_redirection(char *tokens[], const char *output_file);
void global_usage();
void quit_program();

// Process management functions for background processes
void add_process(pid_t pid);
void remove_process(pid_t pid);
void print_running_processes();
void check_finished_processes();

#endif
