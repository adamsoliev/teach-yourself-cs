/*
Shell
    [DONE] 1. Creating the child process and executing the command in the child
    [DONE] 2. Providing a history feature
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

// ------------------------------ DEQUE ------------------------------
#define QUEUE_SIZE 10

typedef struct {
    int front;
    int rear;
    int count;
    char *items[QUEUE_SIZE];
} Queue;

// USAGE
// Queue q; // history
// init(&q);
// enqueue(&q, "item7");
// printf("Dequeued: %s\n", dequeue(&q));

void init(Queue *q) {
    q->front = 0;
    q->rear = 0;
    q->count = 0;
}

int is_empty(Queue *q) { return q->count == 0; }

int is_full(Queue *q) { return q->count == QUEUE_SIZE; }

void enqueue(Queue *q, char *item) {
    if (is_full(q)) {
        free(q->items[q->front]);
        q->front = (q->front + 1) % QUEUE_SIZE;
        q->count--;
    }
    char *copy = malloc(strlen(item) + 1);
    if (copy == NULL) {
        printf("Failed to allocate memory for queue element!\n");
        return;
    }
    strcpy(copy, item);
    q->items[q->rear] = copy;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->count++;
}

char *dequeue(Queue *q) {
    if (is_empty(q)) {
        printf("Queue is empty!\n");
        return NULL;
    }

    char *item = q->items[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    return item;
}

void print_queue(Queue *q) {
    int start = q->front, end = q->rear;
    for (int i = start; i < end; i++) {
        printf("%s\n", q->items[i]);
    }
}
// --------------------------- DEQUE END ---------------------------

// ---------------------------- PROGRAM ----------------------------
#define MAX_LINE 80 // the maximum length command

void tokenize(const char *string, char *delimiter, char **tokens,
              size_t *num_tokens, size_t max_tokens_sz) {
    assert(string != NULL);    // ensure string is a valid pointer
    assert(delimiter != NULL); // ensure delimiter is a valid pointer
    assert(tokens != NULL);    // ensure tokens array is a valid pointer
    assert(max_tokens_sz > 0); // ensure array has at least some space

    char *str_copy = malloc(strlen(string) + 1);
    strcpy(str_copy, string);
    assert(str_copy != NULL); // ensure str_copy is a valid pointer

    char *token = strtok(str_copy, delimiter);
    size_t index = 0;
    while (token != NULL) {
        // ensure tokens array is large enough
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
    size_t new_array_length = array_length + 1;
    size_t new_array_last_element = array_length;

    if (concurrent) { // array has unnecessary '&' at the end
        new_array_length = array_length;
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
    // replace either '\n' or '\0' with '\0'
    string[strcspn(string, "\n")] = 0;
}

void run_command(char **args, size_t num_tokens) {
    assert(args != NULL);
    assert(num_tokens > 0);

    // Copy and add NULL
    bool concurrent = (strcmp(args[num_tokens - 1], "&") == 0);

    if (concurrent)
        printf("Child process is running concurrently\n");

    char **args_with_null = add_null(args, num_tokens, concurrent);

    pid_t child_pid = fork();
    if (child_pid < 0) {
        // fork error
        fprintf(stderr, "Fork failed\n");
    } else if (child_pid == 0) {
        // child process
        char path_to_program[] = "/bin/";
        char *program = args_with_null[0];
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
// -------------------------- PROGRAM END --------------------------

// --------------------------- UTILS ---------------------------
void print_array(char *str, char **array, size_t size) {
    assert(str != NULL);
    assert(array != NULL);
    for (size_t i = 0; i < size; i++) {
        printf("%s: ", str);
        printf("%s\n", array[i]);
    }
}

void print_test_result(bool passed, const char *function_name) {
    assert(function_name != NULL);
    if (passed)
        printf("[PASSED] %s()\n", function_name);
    else
        printf("[FAILED] %s()\n", function_name);
}
// ------------------------ UTILS END --------------------------

// --------------------------- TESTS ---------------------------
void test_concurrent_off__add_null() {
    char *array[] = {"one", "two", "three"};
    size_t array_length = 3;
    char **new_array = add_null(array, array_length, false);

    assert(new_array[0] == array[0]);
    assert(new_array[3] == NULL);
    free(new_array);
    print_test_result(true, __FUNCTION__);
};

void test_concurrent_on__add_null() {
    char *array[] = {"one", "two", "three", "&"};
    size_t array_length = 4;
    char **new_array = add_null(array, array_length, true);

    assert(new_array[0] == array[0]);
    assert(new_array[3] == NULL);
    free(new_array);
    print_test_result(true, __FUNCTION__);
};

void test_end_with_newline__remove_newline() {
    char str[] = "string\n"; // this can't be string literal as it is read-only
    remove_newline(str);
    assert(strcmp(str, "string") == 0);
    print_test_result(true, __FUNCTION__);
}

void test_end_with_null__remove_newline() {
    char str[] = "string";
    remove_newline(str);
    assert(strcmp(str, "string") == 0);
    print_test_result(true, __FUNCTION__);
}

// ------------------------- TESTS END -------------------------

void run_test_suite() {
    test_concurrent_off__add_null();
    test_concurrent_on__add_null();
    test_end_with_newline__remove_newline();
    test_end_with_null__remove_newline();
}

int main(void) {
    run_test_suite();

    char input[100];
    char *args[(MAX_LINE / 2) + 1]; // command line arguments
    int should_run = 1;             // flag to determine when to exit program

    Queue q; // history
    init(&q);

    while (should_run) {
        printf("osh> ");
        fflush(stdout); // ensure "osh>" is printed

        fgets(input, sizeof(input), stdin);
        remove_newline(input);

        // push input to history
        enqueue(&q, input);

        // check for exit
        if (strcmp(input, "exit") == 0) {
            break;
        } else if (strcmp(input, "hist") == 0) {
            print_queue(&q);
            continue;
        }

        // Tokenize and put results into args
        size_t num_tokens;
        tokenize(input, " ", args, &num_tokens, (size_t)(MAX_LINE / 2) + 1);

        run_command(args, num_tokens);
    }
    return 0;
}
