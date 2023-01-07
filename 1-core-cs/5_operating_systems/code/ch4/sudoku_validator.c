
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 9
#define COLS 9
#define NUM_BOARDS 15
#define MAXVAL 45

#define RESET "\033[0m"
#define RED "\033[31m"

// Function prototypes
void generate_random_grid(int grid[ROWS][COLS]);
void print_grid(int grid[ROWS][COLS]);
void sum_rows(int grid[ROWS][COLS]);
void sum_columns(int grid[ROWS][COLS]);
void sum_3x3_grids(int grid[ROWS][COLS]);
void print_stats(int grid[ROWS][COLS]);
int is_valid(int board[ROWS][COLS], int row, int col, int val);
int check_board(int board[ROWS][COLS]);
void read_boards(char *filename, int boards[NUM_BOARDS][ROWS][COLS]);

// Function definitions
void generate_random_grid(int grid[ROWS][COLS]) {
    // Seed random number generator
    srand((unsigned)clock());
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // Generate a random int between 1-9
            grid[i][j] = rand() % 9 + 1;
        }
    }
}

void print_grid(int grid[ROWS][COLS]) {
    // Print the grid
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

void sum_rows(int grid[ROWS][COLS]) {
    int sum;
    printf("rows\t:");
    for (int i = 0; i < ROWS; i++) {
        sum = 0;
        for (int j = 0; j < COLS; j++) {
            sum += grid[i][j];
        }
        if (sum != MAXVAL) {
            printf(RED "%d" RESET, sum); // red text
        } else {
            printf("%d", sum);
        }
        if (i < ROWS - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

void sum_columns(int grid[ROWS][COLS]) {
    int sum;
    printf("columns\t:");
    for (int j = 0; j < COLS; j++) {
        sum = 0;
        for (int i = 0; i < ROWS; i++) {
            sum += grid[i][j];
        }
        if (sum != MAXVAL) {
            printf(RED "%d" RESET, sum); // red text
        } else {
            printf("%d", sum);
        }
        if (j < COLS - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

void sum_3x3_grids(int grid[ROWS][COLS]) {
    int sum;
    printf("3x3s\t:");
    for (int i = 0; i < ROWS; i += 3) {
        for (int j = 0; j < COLS; j += 3) {
            sum = 0;
            for (int x = i; x < i + 3; x++) {
                for (int y = j; y < j + 3; y++) {
                    sum += grid[x][y];
                }
            }
            if (sum != MAXVAL) {
                printf(RED "%d" RESET, sum); // red text
            } else {
                printf("%d", sum);
            }
            if (i < ROWS - 3 || j < COLS - 3) {
                printf(", ");
            }
        }
    }
    printf("\n");
}

void print_stats(int grid[ROWS][COLS]) {
    sum_rows(grid);
    sum_columns(grid);
    sum_3x3_grids(grid);
    if (check_board(grid)) {
        printf("VALID BOARD\n");
    } else {
        printf(RED "INVALID BOARD" RESET "\n");
    }
    printf("\n");
}

int is_valid(int board[ROWS][COLS], int row, int col, int val) {
    // Check if the value is already present in the row
    for (int j = 0; j < COLS; j++) {
        if (j != col && board[row][j] == val) {
            return 0;
        }
    }

    // Check if the value is already present in the column
    for (int i = 0; i < ROWS; i++) {
        if (row != i && board[i][col] == val) {
            return 0;
        }
    }

    // Check if the value is already present in the 3x3 grid
    int grid_row = row - row % 3;
    int grid_col = col - col % 3;
    for (int i = grid_row; i < grid_row + 3; i++) {
        for (int j = grid_col; j < grid_col + 3; j++) {
            if ((row != i && col != j) && board[i][j] == val) {
                return 0;
            }
        }
    }

    // If no conflicts were found, the value is valid
    return 1;
}

int check_board(int board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (!is_valid(board, i, j, board[i][j])) {
                return 0;
            }
        }
    }
    return 1;
}

void read_boards(char *filename, int boards[NUM_BOARDS][ROWS][COLS]) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }

    for (int k = 0; k < NUM_BOARDS; k++) {
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                fscanf(fp, "%d", &boards[k][i][j]);
            }
        }
    }

    fclose(fp);
}

/* structure for passing data to threads */
typedef struct {
    int row;
    int col;
} parameters;

int main() {
    // int grid[ROWS][COLS];
    // for (int i = 0; i < 20; ++i) {
    //     if (i % 2 == 0) {
    //         printf("GRID\n");
    //         generate_random_grid(grid);
    //     } else {
    //         printf("BOARD\n");
    //         generate_random_board(grid);
    //     }
    //     print_grid(grid);
    //     print_stats(grid);
    // }

    int boards[NUM_BOARDS][ROWS][COLS];
    read_boards("sudoku_solutions.txt", boards);
    for (int i = 0; i < NUM_BOARDS; ++i) {
        // int board[ROWS][COLS] = boards[i];
        print_grid(boards[i]);
        print_stats(boards[i]);
    }

    // parameters *data = (parameters *)malloc(sizeof(parameters));
    // data->row = 1;
    // data->col = 1;
    // Create a thread and pass it data as a parameter
    // Collect the results and return the result

    return 0;
}
