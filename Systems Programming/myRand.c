#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define NUM_VALUES 60
#define MAX_RAND 100
#define FILENAME_SIZE 12
#define BUFFER_SIZE 10  

int main() {
    int fd;
    int randomValue;
    char filename[FILENAME_SIZE];
    char buffer[BUFFER_SIZE];
    int randomX;
    ssize_t writtenBytes;

    srand(time(NULL));

    randomX = rand() % 256;

    snprintf(filename, FILENAME_SIZE, "data%d.dat", randomX);

    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening the file");
        return 1; 
    }

    printf("Creating file: %s\n", filename);

    for (int i = 0; i < NUM_VALUES; i++) {
        randomValue = rand() % (MAX_RAND + 1); 

        snprintf(buffer, BUFFER_SIZE, "%d\n", randomValue);

        writtenBytes = write(fd, buffer, strlen(buffer));
        if (writtenBytes == -1) {
            perror("Error writing to the file");
            close(fd);
            return 1; 
        }

        printf("Written Value %d: %d\n", i + 1, randomValue);
    }

   if (close(fd) == -1) {
        perror("Error closing the file");
        return 1; 
    }

    exit(randomX);
}