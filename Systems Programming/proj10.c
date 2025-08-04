#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void child(int pipefd[]) {
    close(pipefd[0]);

    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }
    close(pipefd[1]);

    execlp("./rand", "rand", "-c", "50", "-m", "200", "-M", "500", (char *)NULL);
    perror("execlp");
    exit(EXIT_FAILURE);
}

void parent(int pipefd[]) {
    close(pipefd[1]);

    int value;
    int sum = 0;
    int count = 0;
    
    while (read(pipefd[0], &value, sizeof(int)) == sizeof(int)) {
        printf("Parent read: %d\n", value);
        sum += value;
        count++;
    }

    if (count > 0) {
        double average = (double)sum / count;
        printf("Total: %d\n", sum);
        printf("Average: %.2f\n", average);
    } else {
        printf("No integers were read.\n");
    }

    close(pipefd[0]);
}

int main() {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  
        child(pipefd);
    } else {      
        parent(pipefd);
        wait(NULL);
    }

    return 0;
}
