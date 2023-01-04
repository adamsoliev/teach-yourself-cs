
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LINE 80 /* 80 chars per line, per command */
#define DELIMITERS " \t\n\v\f\r"

// initialize args array, making all of its content NULL
void init_args(char *args[]) {
    for (size_t i = 0; i != MAX_LINE / 2 + 1; i++) {
        args[i] = NULL;
    }
}

// initialize command, making it an empty string
void init_command(char *command) { strcpy(command, ""); }

// free old content and set to NULL
void refresh_args(char *args[]) {
    while (*args) {
        free(*args); // to avoid memory leaks
        *args++ = NULL;
    }
}

// parse input (tokenize) and store results
size_t parse_input(char *args[], char *original_command) {
    size_t num = 0;
    char command[MAX_LINE + 1];
    strcpy(command,
           original_command); // make a copy since 'strtok' will modify it

    char *token = strtok(command, DELIMITERS);
    while (token != NULL) {
        args[num] = malloc(strlen(token) + 1);
        strcpy(args[num], token);
        ++num;
        token = strtok(NULL, DELIMITERS);
    }
    return num;
}

// get command from input of history
int get_input(char *command) {
    char input_buf[MAX_LINE + 1];
    if (fgets(input_buf, MAX_LINE + 1, stdin) == NULL) {
        fprintf(stderr, "Failed to read input!\n");
        return 0;
    }
    if (strncmp(input_buf, "!!", 2) == 0) {
        if (strlen(command) == 0) {
            fprintf(stderr, "No history available yet!\n");
            return 0;
        }
        printf("%s", command); // keep the command unchanged and print it
        return 1;
    }
    strcpy(command, input_buf); // update the command
    return 1;
}

// check for '&'; if so, remove it from args and reduce its size
int check_ampersand(char **args, size_t *size) {
    size_t len = strlen(args[*size - 1]);
    if (args[*size - 1][len - 1] != '&') {
        return 0;
    }
    if (len == 1) {
        free(args[*size - 1]);
        args[*size - 1] = NULL;
        --(*size);
    } else {
        args[*size - 1][len - 1] = '\0';
    }
    return 1;
}

// check redirection tokens in args and remove them
unsigned check_redirection(char **args, size_t *size, char **input_file,
                           char **output_file) {
    unsigned flag = 0;
    size_t to_remove[4], remove_cnt = 0;
    for (size_t i = 0; i != *size; ++i) {
        if (remove_cnt >= 4) {
            break;
        }
        if (strcmp("<", args[i]) == 0) { // input
            to_remove[remove_cnt++] = i;
            if (i == (*size) - 1) {
                fprintf(stderr, "No input file provided!\n");
                break;
            }
            flag |= 1;
            *input_file = args[i + 1];
            to_remove[remove_cnt++] = ++i;
        } else if (strcmp(">", args[i]) == 0) { // output
            to_remove[remove_cnt++] = i;
            if (i == (*size) - 1) {
                fprintf(stderr, "No output file provided!\n");
                break;
            }
            flag |= 2;
            *output_file = args[i + 1];
            to_remove[remove_cnt++] = ++i;
        }
    }
    /* Remove I/O indicators and filenames from arguments */
    for (int i = remove_cnt - 1; i >= 0; --i) {
        size_t pos = to_remove[i]; // inx of arg to remove
        while (pos != *size) {
            args[pos] = args[pos + 1];
            ++pos;
        }
        --(*size);
    }
    return flag;
}

// open files and redirect I/O
int redirect_io(unsigned io_flag, char *input_file, char *output_file,
                int *input_desc, int *output_desc) {
    if (io_flag & 2) { // redirecting output
        *output_desc = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 644);
        if (*output_desc < 0) {
            fprintf(stderr, "Failed to open the output file %s\n", output_file);
            return 0;
        }
        dup2(*output_desc, STDOUT_FILENO);
    }
    if (io_flag & 1) { // redirecting input
        *input_desc = open(output_file, O_RDONLY, 0644);
        if (*input_desc < 0) {
            fprintf(stderr, "Failed to open the input file %s\n", output_file);
            return 0;
        }
        dup2(*input_desc, STDIN_FILENO);
    }
    return 1;
}

// close files for input and output
void close_file(unsigned io_flag, int input_desc, int output_desc) {
    if (io_flag & 2) {
        close(output_desc);
    }
    if (io_flag & 1) {
        close(input_desc);
    }
}

// detect the pipe '|' and split args into two parts accordingly
void detect_pipe(char **args, size_t *args_num, char ***args2,
                 size_t *args_num2) {
    for (size_t i = 0; i != *args_num; ++i) {
        if (strcmp(args[i], "|") == 0) {
            free(args[i]);
            args[i] = NULL;
            *args_num2 = *args_num - i - 1;
            *args_num = i;
            *args2 = args + i + 1;
            break;
        }
    }
}

// run command
int run_command(char **args, size_t args_num) {
    int run_concurrently = check_ampersand(args, &args_num);
    char **args2;
    size_t args_num2 = 0;
    detect_pipe(args, &args_num, &args2, &args_num2);
    pid_t pid = fork();
    if (pid < 0) { // fork failed
        fprintf(stderr, "Failed to fork!\n");
        return 0;
    } else if (pid == 0) {    // child process
        if (args_num2 != 0) { // pipe
            /* Create a pipe */
            int fd[2];
            pipe(fd);
            pid_t pid2 = fork();
            /* Fork into another two processes */
            if (pid2 > 0) { // child process for 2nd command
                char *input_file, *output_file;
                int input_desc, output_desc;
                unsigned io_flag = check_redirection(args2, &args_num2,
                                                     &input_file, &output_file);
                io_flag &= 2;
                if (redirect_io(io_flag, input_file, output_file, &input_desc,
                                &output_desc) == 0) {
                    return 0;
                }
                close(fd[1]);
                dup2(fd[0], STDIN_FILENO);
                wait(NULL);
                execvp(args2[0], args2);
                close_file(io_flag, input_desc, output_desc);
                close(fd[0]);
                fflush(stdin);
            } else if (pid2 == 0) { // grandchild process for 1st command
                char *input_file, *output_file;
                int input_desc, output_desc;
                unsigned io_flag = check_redirection(args, &args_num,
                                                     &input_file, &output_file);
                io_flag &= 1;
                if (redirect_io(io_flag, input_file, output_file, &input_desc,
                                &output_desc) == 0) {
                    return 0;
                }
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                execvp(args[0], args);
                close_file(io_flag, input_desc, output_desc);
                close(fd[1]);
                fflush(stdin);
            }
        } else {
            char *input_file, *output_file;
            int input_desc, output_desc;
            unsigned io_flag =
                check_redirection(args, &args_num, &input_file, &output_file);
            if (redirect_io(io_flag, input_file, output_file, &input_desc,
                            &output_desc) == 0) {
                return 0;
            }
            execvp(args[0], args);
            close_file(io_flag, input_desc, output_desc);
            fflush(stdin);
        }
    } else { // parent process
        if (!run_concurrently) {
            wait(NULL);
        }
    }
    return 1;
}

int main(void) {
    char *args[MAX_LINE / 2 +
               1]; /* command line (of 80) has max of 40 arguments*/
    char command[MAX_LINE + 1];
    init_args(args);
    init_command(command);
    while (1) {
        printf("osh>");
        fflush(stdout);
        fflush(stdin);

        /* Make args empty before parsing */
        refresh_args(args);

        /* Get input and parse it */
        if (!get_input(command)) {
            continue;
        }

        size_t args_num = parse_input(args, command);

        /* Continue or exit */
        if (args_num == 0) {
            printf("Please enter the command! (or type \"exit\" to exit)\n");
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        /* Run command */
        run_command(args, args_num);
    }
    refresh_args(args); // to avoid memory leaks!
    return 0;
}