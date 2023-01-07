
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 9
#define COLS 9
#define NUM_BOARDS 15     // comes from txt file
#define VALID_UNIT_SUM 45 // sum of each valid row, col and 3x3 grid
#define NUM_THREADS 11

#define RESET "\033[0m"
#define RED "\033[31m"

/* ------------------- FOR TESTING ------------------- */
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
        if (sum != VALID_UNIT_SUM) {
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
        if (sum != VALID_UNIT_SUM) {
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
            if (sum != VALID_UNIT_SUM) {
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
        printf("SINGLE-THREAD: VALID\n");
    } else {
        printf(RED "SINGLE-THREAD: INVALID" RESET "\n");
    }
    // printf("\n");
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
/* ------------------- FOR TESTING END ------------------- */

/* ------------------- PROGRAM ------------------- */
int grid[ROWS][COLS];
bool region_validity[NUM_THREADS];

void *check_columns(void *arg);
void *check_rows(void *arg);
void *check_subgrid(void *arg);

int main() {
    // Read in the Sudoku boards
    int boards[NUM_BOARDS][ROWS][COLS];
    read_boards("sudoku_solutions.txt", boards);
    for (int i = 0; i < NUM_BOARDS; ++i) {

        /* ----------- SINGLE-THREADED ----------- */
        print_grid(boards[i]);  // prints the whole grid
        print_stats(boards[i]); // prints row, col and subgrid sums and checks
                                // if the board is valid

        // Copy [i]th board of the board array into 'grid'
        for (int a = 0; a < ROWS; a++) {
            for (int b = 0; b < COLS; b++) {
                grid[a][b] = boards[i][a][b];
            }
        }

        /* ----------- MULTI-THREADED ----------- */
        // Initialize the worker threads array
        pthread_t threads[NUM_THREADS];

        // Create a thread to check columns
        pthread_create(&threads[0], NULL, check_columns, NULL);

        // Create a thread to check rows
        pthread_create(&threads[1], NULL, check_rows, NULL);

        // Create nine threads to check each of the subgrids
        for (int i = 0; i < 9; i++) {
            pthread_create(&threads[i + 2], NULL, check_subgrid, (void *)i);
        }

        // Wait for all threads to finish
        for (int i = 0; i < NUM_THREADS; i++) {
            pthread_join(threads[i], NULL);
        }

        // Check if all regions are valid
        bool valid = true;
        for (int i = 0; i < NUM_THREADS; i++) {
            if (!region_validity[i]) {
                valid = false;
                break;
            }
        }
        if (valid)
            printf("MULTI-THREAD: VALID\n");
        else
            printf(RED "MULTI-THREAD: INVALID\n" RESET);
        printf("\n");
    }

    return 0;
}

void *check_columns(void *arg) { // we pass this to 'not cast this function to
                                 // void* in pthread_create'
    // Iterate through each column
    for (int col = 0; col < COLS; col++) {
        bool digits[COLS] = {false};
        // Iterate through each element in the column
        for (int row = 0; row < ROWS; row++) {
            int num = grid[row][col];
            if ((num < 1 || num > 9) || digits[num - 1]) {
                // Invalid Sudoku solution
                // printf("invalid col (row): %d (%d)\n", col, row);
                region_validity[0] = false;
                pthread_exit(0);
            }
            digits[num - 1] = true;
        }
    }
    // All columns are valid
    region_validity[0] = true;
    pthread_exit(0);
}

void *check_rows(void *arg) {
    // Iterature through each row
    for (int row = 0; row < ROWS; row++) {
        bool digits[ROWS] = {false};
        // Iterature through each element in the row
        for (int col = 0; col < COLS; col++) {
            int num = grid[row][col];
            if ((num < 1 || num > 9) || digits[num - 1]) {
                // Invalid Sudoku solution
                // printf("invalid row (col): %d (%d)\n", row, col);
                region_validity[1] = false;
                pthread_exit(0);
            }
            digits[num - 1] = true;
        }
    }
    // All rows are valid
    region_validity[1] = true;
    pthread_exit(0);
}

void *check_subgrid(void *arg) {
    int subgrid_idx = (int)arg;
    // Calculate the starting row and column for the subgrid
    int start_row = (subgrid_idx / 3) * 3;
    int start_col = (subgrid_idx % 3) * 3;

    bool digits[9] = {false};
    // Iterature through each subgrid
    for (int row = start_row; row < start_row + 3; row++) {
        for (int col = start_col; col < start_col + 3; col++) {
            int num = grid[row][col];
            if ((num < 1 || num > 9) || digits[num - 1]) {
                // Invalid Sudoku solution
                region_validity[subgrid_idx + 2] = false;
                pthread_exit(0);
            }
            digits[num - 1] = true;
        }
    }
    // All subgrids are valid
    region_validity[subgrid_idx + 2] = true;
    pthread_exit(0);
}
/* ------------------- PROGRAM END ------------------- */
