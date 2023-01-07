
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 9
#define COLS 9

/* -------------------- FOR TESTING -------------------- */
void generate_random_grid(int grid[ROWS][COLS]) {
    // Seed random number generator
    srand(time(0));
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
        printf("%d", sum);
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
        printf("%d", sum);
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
            printf("%d", sum);
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
}
/* -------------------- FOR TESTING END -------------------- */

/* structure for passing data to threads */
typedef struct {
    int row;
    int col;
} parameters;

int main() {
    int grid[ROWS][COLS];
    generate_random_grid(grid);
    print_grid(grid);
    print_stats(grid);

    // parameters *data = (parameters *)malloc(sizeof(parameters));
    // data->row = 1;
    // data->col = 1;
    // Create a thread and pass it data as a parameter
    // Collect the results and return the result

    return 0;
}
