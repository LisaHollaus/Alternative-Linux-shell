#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    char input[1024];
    while (1) {
        printf("user@host> ");
        fgets(input, 1024, stdin); // Read input from stdin
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // Exit loop if no input
        }
        // Remove newline character from input
        //input[strcspn(input, "\n")] = 0;
        
        // Execute command without system
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
    return count;
}







// int main()
// {
//     pid_t pid;

//     pid = fork();

//     if (pid < 0) {
//         fprintf(stderr, "Fork failed");
//         return 1;
//     }
//     else if (pid == 0) {
//         printf("Child: Executing ls...\n");
//         system("/bin/ls");
//         printf("Child: Execution complete\n");
//     }
//     else {
//         wait(NULL);
//         printf("Parent: Child complete\n");
//         printf("Value of PID of the child was %d\n", pid);
//     }
// }
