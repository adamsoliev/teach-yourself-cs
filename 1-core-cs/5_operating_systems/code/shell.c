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
    assert(string != NULL);    // make sure string is a valid pointer
    assert(delimiter != NULL); // make sure delimiter is a valid pointer
    assert(tokens != NULL);    // make sure tokens array is a valid pointer
    assert(max_tokens_sz > 0); // make sure array has at least some space

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

char **add_null(char **array, size_t array_length) {
    assert(array != NULL);
    assert(array_length > 0);
    char **new_array = malloc((array_length + 1) * sizeof(char *));
    for (size_t i = 0; i < array_length; i++) {
        new_array[i] = array[i];
    }
    new_array[array_length] = NULL;
    return new_array;
}

void remove_newline(char *string) {
    assert(string != NULL);
    string[strcspn(string, "\n")] = 0; // replace either '\n' or '\0' with '\0'
}

void run_command(char **args, size_t num_tokens) {
    // Copy and add NULL
    char **args_with_null = add_null(args, num_tokens);

    pid_t child_pid = fork();
    if (child_pid < 0) {
        // fork error
        fprintf(stderr, "Fork failed\n");
    } else if (child_pid == 0) {
        // child process
        char path_to_program[] = "/bin/";
        char *program          = args_with_null[0];
        strcat(path_to_program, program);

        // Execute the program
        execv(path_to_program, args_with_null);
        // execvp(args[0], args);

        printf("Child process\n");
    } else {
        // parent process
        int status;
        waitpid(child_pid, &status, 0);
        printf("Child process terminated with status %d\n", status);
    }
    free(args_with_null);
}

void print_array(char *str, char **array, size_t size) {
    assert(str != NULL);
    assert(array != NULL);
    for (size_t i = 0; i < size; i++) {
        printf("%s: ", str);
        printf("%s\n", array[i]);
    }
}

int main(void) {
    char input[100];
    char *args[(MAX_LINE / 2) + 1]; // command line arguments
    int should_run = 1;             // flag to determine when to exit program
    while (should_run) {
        printf("osh> ");
        fflush(stdout); // ensure "osh>" is printed

        fgets(input, sizeof(input), stdin);
        remove_newline(input);

        // check for exit
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Tokenize and put results into args
        size_t num_tokens;
        tokenize(input, " ", args, &num_tokens, (size_t)(MAX_LINE / 2) + 1);

        run_command(args, num_tokens);

        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) parent will invoke wait() unless command included &
         */
    }
    return 0;
}
