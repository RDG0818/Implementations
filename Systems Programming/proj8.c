#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define NUM_VALUES 60
#define FILENAME_SIZE 12
#define BUFFER_SIZE 32  

int main() {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        execl("./myRand", "./myRand", (char *) NULL);
        perror("exec failed");
        exit(1);
    } else {
        wait(&status);

        int randomX = WEXITSTATUS(status);

        char filename[FILENAME_SIZE];
        snprintf(filename, FILENAME_SIZE, "data%d.dat", randomX);

        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("Error opening the data file");
            exit(1);
        }

        int value;
        double total = 0;
        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;
        int numbers[NUM_VALUES];
        int i = 0;  

        printf("Reading values from file %s:\n", filename);

        while (i < NUM_VALUES && (bytesRead = read(fd, buffer, BUFFER_SIZE - 1)) > 0) {
            buffer[bytesRead] = '\0'; 

            char *token = strtok(buffer, "\n");
            while (token != NULL && i < NUM_VALUES) {
                printf("Value %d: %s\n", i + 1, token);
                if (sscanf(token, "%d", &numbers[i]) == 1) {
                    i++;
                }
                token = strtok(NULL, "\n"); 
            }
            
        }

        for (int j = 0; j < NUM_VALUES; j++) {
            total += numbers[j];
        }
        double average = (total/NUM_VALUES);
        printf("Average Value: %f\n", average);

        close(fd);

        if (unlink(filename) == -1) {
            perror("Error deleting the file");
            exit(1);
        }

    }

    return 0;
}