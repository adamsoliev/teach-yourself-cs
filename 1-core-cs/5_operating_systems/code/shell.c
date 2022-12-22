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

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 80 // the maximum length command

int main(void) {
    char input[100];
    char *args[(MAX_LINE / 2) + 1]; // command line arguments
    int should_run  = 1;            // flag to determine when to exit program
    int token_index = 0;
    while (should_run) {

        printf("osh>");
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

        // tokenize
        args[token_index] = strtok(input, " ");
        while (args[token_index] != NULL) {
            // printf("token at %d index is %s\n", token_index,
            // args[token_index]);
            token_index++;
            fflush(stdout);
            args[token_index] = strtok(NULL, " ");
        }

        // printf("The number of tokens: %d\n", token_index);

        // extract command
        char *command = args[0];

        // extract command options
        int num_options = token_index - 1 + 1; // remove program and add NULL
        char *options[num_options];
        // memcpy(options, args + 1, sizeof(char *) * token_index - 1);
        memcpy(options, args, sizeof(char *) * (token_index + 1));
        options[token_index] = NULL; // end

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
            // execlp("/bin/ls", "ls", NULL);
            // execvp(command, options);
            char path_to_program[] = "/bin/";
            char *program          = args[0];
            strcat(path_to_program, program);
            execv(path_to_program, options);
            printf("Child process\n");
        } else {
            // parent process
            int status;
            waitpid(child_pid, &status, 0);
            printf("Child process terminated with status %d\n", status);
        }

        token_index = 0;
        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) parent will invoke wait() unless command included &
         */
    }
    return 0;
}

int run_child_process() {
    pid_t pid;
    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    } else if (pid == 0) { /* child process */
        printf("Child Proc-pedanticess Running");
    } else { /* parent process */
        /* parent will wait for the child to complete */
        // wait(NULL);
        printf("Child Complete");
    }
    return 0;
}
