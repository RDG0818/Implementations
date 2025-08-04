#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#define LENGTH 256

int timer_paused = 0;
struct itimerval saved_timer;
int quiz_running = 1;

void start_timer() {
    if (!timer_paused) {
            struct itimerval timer;
        timer.it_value.tv_sec = 15; // 15 seconds
        timer.it_value.tv_usec = 0;
        timer.it_interval.tv_sec = 0; 
        timer.it_interval.tv_usec = 0;
        setitimer(ITIMER_REAL, &timer, NULL); 
    }
    else {
        setitimer(ITIMER_REAL, &saved_timer, NULL);
        printf("\nTimer resumed with %ld seconds remaining.\n", saved_timer.it_value.tv_sec);
        timer_paused = 0;
    }
}

void stop_running_timer() {
    getitimer(ITIMER_REAL, &saved_timer);  
    printf("Timer paused with %ld seconds left.\n", saved_timer.it_value.tv_sec);
    timer_paused = 1;
    struct itimerval stop_timer = {0};
    setitimer(ITIMER_REAL, &stop_timer, NULL);
}

void stop_timer() {
    struct itimerval timer = {0}; // Stop the timer
    setitimer(ITIMER_REAL, &timer, NULL);
}

void sigint_handler(int signum) {
    char response[3]; 

    stop_running_timer();

    write(STDOUT_FILENO, "\nExit (y/N)? ", 13);

    ssize_t bytes_read = read(STDIN_FILENO, response, 2);
    if (bytes_read > 0) {
        
        response[bytes_read] = '\0'; // Null-terminate the string
        
        if (response[0] == 'y' || response[0] == 'Y') {
            quiz_running = 0; 
            close(STDIN_FILENO);
        } else {
            write(STDOUT_FILENO, "Resuming quiz...\n", 17);
            start_timer(); 
        }
    }
}

ssize_t read_line(int fd, char *buffer, size_t max_length) {
    ssize_t bytes_read;
    size_t total_read = 0;

    while (total_read < max_length - 1) {
        bytes_read = read(fd, buffer + total_read, 1);  
        if (bytes_read <= 0) {
            break;
        }

        if (buffer[total_read] == '\n') {
            break;
        }

        total_read += bytes_read;
    }

    buffer[total_read] = '\0';
    return total_read;
}

volatile sig_atomic_t timed_out = 0;

void timer_handler(int signum) {
    timed_out = 1; // Set the timed_out flag
}

int main() {
    printf("You are about to start a timed quiz. You have 15 seconds per question. You can exit at any time using CTRL + C. Do you wish to proceed?\n");
    char question[LENGTH];
    char answer[LENGTH];
    char user_input[LENGTH];

    struct sigaction sa;
    sa.sa_handler = timer_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    struct sigaction sa_int;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    int total_questions = 0;
    int correct_answers = 0;
    read(STDIN_FILENO, user_input, sizeof(user_input));
    if (user_input[0] != 'y' && user_input[0] != 'Y') {
        printf("Exiting the quiz.\n");
        return 0;
    }
    int q_fd = open("quest.txt", O_RDONLY);
    int a_fd = open("ans.txt", O_RDONLY);

    if (q_fd < 0 || a_fd < 0) {
        perror("Error opening file");
        return 1;
    }

    while (quiz_running && (read_line(q_fd, question, sizeof(question) - 1) > 0 && read_line(a_fd, answer, sizeof(answer) - 1) > 0)) {
        total_questions++;
        question[strcspn(question, "\n")] = 0;
        answer[strcspn(answer, "\n")] = 0;

        printf("\nQuestion: %s\n", question);
        printf("Your answer: \n");
        timed_out = 0;
        start_timer();
        int bytes = read(STDIN_FILENO, user_input, sizeof(user_input) - 1);
        stop_timer();
        if (bytes > 0) {
            user_input[bytes - 1] = '\0'; // Null-terminate the string
        }
        if (timed_out) {
            printf("Time has elapsed.\n");
        }
        else {
            if (strcmp(user_input, answer) == 0) {
                printf("Correct.\n");
                correct_answers++;
            } else if (!quiz_running) {
                printf("Exiting the program.\n");
            } 
            else {
                printf("Wrong! The correct answer was: %s\n", answer);
            }
        }
    }

    

    close(q_fd);
    close(a_fd);

    printf("\nQuiz finished! You answered %d out of %d questions correctly.\n",
           correct_answers, total_questions);
    return 0;

}