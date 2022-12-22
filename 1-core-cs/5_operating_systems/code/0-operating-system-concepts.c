#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// 2.24
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <src file> <dst file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the source file for reading
    int source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1)
    {
        perror("Error openning the source file");
        exit(EXIT_FAILURE);
    }

    // Open the destination file for writing
    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1)
    {
        perror("Error openning the destination file");
        exit(EXIT_FAILURE);
    }

    // Allocate a buffer for reading the source file
    char buffer[BUFFER_SIZE];

    // Read from the source file and write to the destination file
    ssize_t bytes_read;
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0)
    {
        ssize_t bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written == -1)
        {
            perror("Error writing to the destination file");
            exit(EXIT_FAILURE);
        }
    }

    // Check if there was an error while reading the source file
    if (bytes_read == -1)
    {
        perror("Error reading from the source file");
        exit(EXIT_FAILURE);
    }

    // Close the files
    close(source_fd);
    close(dest_fd);

    return 0;
}