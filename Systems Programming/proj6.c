#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>


pid_t child_pid;

struct DataPoint {
    double angle[3];
};

void handle_sigchld(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (pid > 0) {
        printf("Child process %d terminated\n", pid);
    }

    if (pid == -1 && errno == ECHILD) {
        printf("No more children. Parent terminating.\n");
        exit(0);
    }
}

void handle_sigint(int sig) {
    kill(child_pid, SIGSTOP);
    char response;
    printf("\nExit: Are you sure (Y/n)? ");
    response = getchar();
    char discard = getchar();
    if (response == 'Y' || response == 'y') {
        printf("Parent exiting...\n");
        kill(child_pid, SIGTERM);
        exit(0);
    }
    else {
        printf("Continuing child...\n");

        kill(child_pid, SIGCONT);
    }
}

void handle_sigusr1(int sig) {
    printf("Warning! Roll outside of bounds\n");
}

void handle_sigusr2(int sig) {
    printf("Warning! Pitch outside of bounds\n");
}

void child_process(pid_t parent_pid) {
    const char* file_path = "angl.dat";
    int input_fd = open(file_path, O_RDONLY);
    if (input_fd < 0) {
        perror("Failed to open angl.dat file");
        exit(1);
    }
    struct DataPoint data;
    ssize_t bytesRead;
    struct timespec req = {1, 0};
    int count = 0;
    struct sigaction ignore;
    ignore.sa_handler = SIG_IGN;
    sigemptyset(&ignore.sa_mask);
    ignore.sa_flags = 0;
    sigaction(SIGINT, &ignore, NULL);

    while ((bytesRead = read(input_fd, &data, sizeof(struct DataPoint))) == sizeof(struct DataPoint)) {
        count++;
        printf("Child: Reading data set %d: Roll: %.8f, Pitch: %.8f, Yaw: %.8f\n", count, data.angle[0], data.angle[1], data.angle[2]);

        if (data.angle[0] < -20 || data.angle[0] > 20) {
            kill(parent_pid, SIGUSR1);
        }

        if (data.angle[1] < -20 || data.angle[1] > 20) {
            kill(parent_pid, SIGUSR2);
        }

        nanosleep(&req, NULL);
        printf("\n");
    }

    if (bytesRead < 0) {
        perror("Error reading from angl.dat");
    }

    close(input_fd);
    printf("Child process finished reading from file.\n");
    exit(0);
}

void parent_process() {
    while (1) {
        pause();
    }
}

int main() {
    struct sigaction sa;

    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = handle_sigusr1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_handler = handle_sigusr2;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR2, &sa, NULL);

    child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        exit(1);

    } else if (child_pid == 0) {
        printf("Child process started with PID: %d\n", getpid());
        child_process(getppid());
    } else {
        printf("Parent process started with child PID: %d\n", child_pid);
        parent_process();
    }

    return 0;
}