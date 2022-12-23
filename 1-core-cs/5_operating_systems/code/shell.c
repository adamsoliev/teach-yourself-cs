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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 80 // the maximum length commando

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

char **add_null(char **array, size_t array_length, bool concurrent) {
    assert(array != NULL);
    assert(array_length > 0);
    size_t new_array_length       = array_length + 1;
    size_t new_array_last_element = array_length;

    if (concurrent) { // array has unnecessary '&' at the end
        new_array_length       = array_length;
        new_array_last_element = array_length - 1;
    }
    /*
    {'a', 'b', 'c', '&'}  =>  {'a', 'b', 'c', 'NULL'}
    {'a', 'b', 'c'}       =>  {'a', 'b', 'c', 'NULL'}
    */

    char **new_array = malloc((new_array_length) * sizeof(char *));
    for (size_t i = 0; i < new_array_length; i++) {
        new_array[i] = array[i];
    }
    new_array[new_array_last_element] = NULL;
    return new_array;
}

void remove_newline(char *string) {
    assert(string != NULL);
    string[strcspn(string, "\n")] = 0; // replace either '\n' or '\0' with '\0'
}

void run_command(char **args, size_t num_tokens) {
    // Copy and add NULL
    bool concurrent = (strcmp(args[num_tokens - 1], "&") == 0);

    if (concurrent)
        printf("Concurrent is on\n");

    char **args_with_null = add_null(args, num_tokens, concurrent);

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
        if (!concurrent) {
            int status;
            waitpid(child_pid, &status, 0);
            printf("Child process terminated with status %d\n", status);
        }
        printf("Parent process\n");
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

void print_test_result(bool passed, const char *function_name) {
    if (passed)
        printf("[PASSED] %s()\n", function_name);
    else
        printf("[FAILED] %s()\n", function_name);
}

// --------------------------- TESTS ---------------------------
void test_concurrent_off__add_null() {
    char *array[]       = {"one", "two", "three"};
    size_t array_length = 3;
    char **new_array    = add_null(array, array_length, false);

    assert(new_array[0] == array[0]);
    assert(new_array[3] == NULL);
    free(new_array);
    print_test_result(true, __FUNCTION__);
};

void test_concurrent_on__add_null() {
    char *array[]       = {"one", "two", "three", "&"};
    size_t array_length = 4;
    char **new_array    = add_null(array, array_length, true);

    assert(new_array[0] == array[0]);
    assert(new_array[3] == NULL);
    free(new_array);
    print_test_result(true, __FUNCTION__);
};

// ------------------------- TESTS END -------------------------

void run_test_suite() {
    test_concurrent_off__add_null();
    test_concurrent_on__add_null();
}

int main(void) {
    run_test_suite();

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
    }
    return 0;
}
