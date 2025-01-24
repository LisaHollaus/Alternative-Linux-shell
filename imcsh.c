#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <functions.h>

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
        input[strcspn(input, "\n")] = 0;
        

        // Tokenize input
        // int count = tokenize(input, tokens);
        
        // Execute command based on first token
        if (strncmp(tokens[0], "exec", 4) == 0) {
            // Remove the "exec" part and pass the rest to execute_command
            execute_command(tokens + 1);
            continue;
    }
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
