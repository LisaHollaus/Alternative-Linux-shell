// Helper functions for imcsh.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


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