#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 20

/*
SIZE
4   [1, 3, 6, 7]

    0 - 1, 2 - 3
        f_start = 0
        f_end   = (SIZE / 2) - 1

5   [1, 3, 6, 7, 0]

    0 - 2, 3 - 4
        s_start = (SIZE / 2)
        s_end   = SIZE - 1;
*/

typedef struct {
    int start;
    int end;
} data;

/* ------------- Multi-thread sorting ------------- */
void *sort_first_half(void *arg);
void *sort_second_half(void *arg);
void generate_random_array();
void merge(int arr[], int start, int mid, int end, int sorted_array[]);
/* ------------- Multi-thread sorting END ------------- */

/* ------------- Quicksort ------------- */
void swap(int *a, int *b);
int partition(int arr[], int low, int high);
void quicksort(int arr[], int low, int high);
/* ------------- Quicksort END ------------- */

/* ------------- Utils ------------- */
void print_array(int arr[], int size);
/* ------------- Utils END ------------- */

int array[SIZE];
int sorted_array[SIZE];

int main(int argc, char *argv[]) {
    // Fill the array
    generate_random_array();

    print_array(array, SIZE);

    // Initialize two sorting threads
    pthread_t sorting_threads[2];
    data d[2];

    // Initialize data for passing to threads
    d[0].start = 0;
    d[0].end = (SIZE / 2) - 1;
    d[1].start = (SIZE / 2);
    d[1].end = SIZE - 1;

    // Create threads
    int first = pthread_create(&sorting_threads[0], NULL, sort_first_half,
                               (void *)&d[0]);
    int second = pthread_create(&sorting_threads[1], NULL, sort_second_half,
                                (void *)&d[1]);

    // TODO: Do error checking

    // Wait for both threads to finish
    pthread_join(sorting_threads[0], NULL);
    pthread_join(sorting_threads[1], NULL);

    // TODO: Move to another thread
    merge(array, 0, (SIZE - 1) / 2, SIZE - 1, sorted_array);

    print_array(sorted_array, SIZE);

    return 0;
}

void *sort_first_half(void *arg) {
    data *d = (data *)arg;
    int start = d->start;
    int end = d->end;
    quicksort(array, start, end);
}

void *sort_second_half(void *arg) {
    data *d = (data *)arg;
    int start = d->start;
    int end = d->end;
    quicksort(array, start, end);
}

void generate_random_array() {
    // Seed random number generator
    srand((unsigned)clock());
    for (int i = 0; i < SIZE; i++) {
        // Generate a random int between 1-100
        array[i] = rand() % 100 + 1;
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low;

    for (int j = low + 1; j <= high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[low], &arr[i]);
    return i;
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivot_index = partition(arr, low, high);
        quicksort(arr, low, pivot_index - 1);
        quicksort(arr, pivot_index + 1, high);
    }
}

void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1)
            printf(", ");
    }
    printf("\n");
}

void merge(int arr[], int start, int mid, int end, int sorted_array[]) {
    int i = start;
    int j = mid + 1;
    int k = 0;
    while (i <= mid && j <= end) {
        if (arr[i] < arr[j]) {
            sorted_array[k++] = arr[i++];
        } else {
            sorted_array[k++] = arr[j++];
        }
    }
    while (i <= mid) {
        sorted_array[k++] = arr[i++];
    }
    while (j <= end) {
        sorted_array[k++] = arr[j++];
    }
}
