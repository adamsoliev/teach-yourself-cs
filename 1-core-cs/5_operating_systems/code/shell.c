/*
shell is a parent process
    listens to user input
    creates either a) or b) type of child process
        a) osh>cat prog.c              // parent waits child to terminate
        b) osh>cat prog.c &            // child runs concurrently

Shell
    1. Creating the child process and executing the command in the child
        parsing what the user has entered into separate tokens
        storing the tokens in an array of character strings

        when the user enters 'ps -ael' at the prompt, the values stored in the
        args array are: args[0] = "ps" args[1] = "-ael" args[2] = NULL

    2. Providing a history feature
    3. Adding support of input and output redirection
    4. Allowing the parent and child processes to communicate via a pipe
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 80 // the maximum length command

void tokenize(const char *string, char *delimiter, char **tokens,
              size_t *num_tokens, size_t max_tokens_sz) {
    assert(string != NULL); // make sure string is a valid pointer
    assert(tokens != NULL); // make sure tokens array is a valid pointer

    char *str_copy = malloc(strlen(string) + 1);
    strcpy(str_copy, string);

    assert(str_copy != NULL); // make sure str_copy is a valid pointer

    char *token  = strtok(str_copy, delimiter);
    size_t index = 0;
    while (token != NULL) {
        // make sure tokens array is large enough
        assert(index < max_tokens_sz);

        tokens[index] = token;
        index++;
        token = strtok(NULL, delimiter);
    }
    *num_tokens = index;
};

int main(void) {
    char input[100];
    char *args[(MAX_LINE / 2) + 1]; // command line arguments
    int should_run = 1;             // flag to determine when to exit program
    while (should_run) {

        printf("osh> ");
        // ensure that whole prev state of stdout buffer is printed
        fflush(stdout);

        fgets(input, sizeof(input), stdin);
        // remove '\n' at the end of input line
        int len    = strcspn(input, "\n");
        input[len] = '\0';

        // check for exit
        if (strcmp(input, "exit") == 0) {
            break;
        }

        char *delimiter = " ";
        size_t num_tokens;
        tokenize(input, delimiter, args, &num_tokens,
                 (size_t)(MAX_LINE / 2) + 1);

        // // tokenize
        // args[token_index] = strtok(input, " ");
        // while (args[token_index] != NULL) {
        //     // printf("token at %d index is %s\n", token_index,
        //     // args[token_index]);
        //     token_index++;
        //     args[token_index] = strtok(NULL, " ");
        // }

        // extract command options
        char *options[num_tokens];
        memcpy(options, args, sizeof(char *) * (num_tokens + 1));
        options[num_tokens] = NULL; // end

        // for (int i = 0; i < num_options; i++) {
        //     printf("option at %d index is %s\n", i, options[i]);
        // }

        // Fork a child
        pid_t child_pid = fork();
        if (child_pid < 0) {
            // fork error
            fprintf(stderr, "Fork failed\n");
            return 1;
        } else if (child_pid == 0) {
            // child process
            char path_to_program[] = "/bin/";
            char *program          = args[0];
            strcat(path_to_program, program);

            // Execute the program
            execv(path_to_program, options);

            printf("Child process\n");
        } else {
            // parent process
            int status;
            waitpid(child_pid, &status, 0);
            printf("Child process terminated with status %d\n", status);
        }

        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) parent will invoke wait() unless command included &
         */
    }
    return 0;
}
