#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#define FIFO1       "/tmp/fifo1"
#define FIFO1_RESP  "/tmp/fifo1_resp"
#define FIFO2       "/tmp/fifo2"
#define FIFO2_RESP  "/tmp/fifo2_resp"

#define MSG_HIGHER    "HIGHER"
#define MSG_LOWER     "LOWER"
#define MSG_WIN       "WIN"
#define MSG_GAMEOVER  "GAMEOVER"

int player1_wins = 0;
int player2_wins = 0;

int read_line(int fd, char *buf, int size) {
    int i = 0;
    char c;
    while (i < size - 1) {
        int n = read(fd, &c, 1);
        if (n <= 0)
            break;
        if (c == '\n')
            break;
        buf[i++] = c;
    }

    buf[i] = '\0';
    return i;
}

void player1() {
    int low = 0, high = 100, guess;
    char buffer[32];
    int guess_fd, resp_fd;

    guess_fd = open(FIFO1, O_WRONLY);
    resp_fd = open(FIFO1_RESP, O_RDONLY);

    if (guess_fd < 0 || resp_fd < 0) {
        perror("player1 open fifo");
        exit(1);
    }

    while (1) {
        guess = (low + high) / 2;
        snprintf(buffer, sizeof(buffer), "%d\n", guess);
        write(guess_fd, buffer, strlen(buffer));
        printf("Player 1 Guess: %d\n", guess);

        int n = read_line(resp_fd, buffer, sizeof(buffer));
        if (n <= 0) break;

        if (strcmp(buffer, MSG_HIGHER) == 0) {
            low = guess;
        } 
        else if (strcmp(buffer, MSG_LOWER) == 0) {
            high = guess;
        } 
        else if (strcmp(buffer, MSG_WIN) == 0) {
            printf("Player 1 Wins!\n");
            exit(0);
        } 
        else if (strcmp(buffer, MSG_GAMEOVER) == 0) {
            exit(0);
        }

        sleep(1);
    }
    close(guess_fd);
    close(resp_fd);
    exit(0);
}

void player2() {
    int low = 0, high = 100, guess;
    char buffer[32];
    int guess_fd, resp_fd;

    guess_fd = open(FIFO2, O_WRONLY);
    resp_fd = open(FIFO2_RESP, O_RDONLY);

    if (guess_fd < 0 || resp_fd < 0) {
        perror("player2 open fifo");
        exit(1);
    }

    srand(getpid());
    while (1) {
        guess = low + rand() % (high - low + 1);
        snprintf(buffer, sizeof(buffer), "%d\n", guess);
        write(guess_fd, buffer, strlen(buffer));
        printf("Player 2 Guess: %d\n", guess);

        int n = read_line(resp_fd, buffer, sizeof(buffer));
        if (n <= 0) break;

        if (strcmp(buffer, MSG_HIGHER) == 0) {
            low = guess;
        } 
        else if (strcmp(buffer, MSG_LOWER) == 0) {
            high = guess;
        } 
        else if (strcmp(buffer, MSG_WIN) == 0) {
            printf("Player 2 Wins!\n");
            exit(0);
        } 
        else if (strcmp(buffer, MSG_GAMEOVER) == 0) {
            exit(0);
        }

        sleep(1);
    }
    close(guess_fd);
    close(resp_fd);
    exit(0);
}

void referee() {
    int target = (rand() % 100) + 1;
    printf("Target Number: %d\n\n", target);

    char buffer1[32], buffer2[32];
    int guess1, guess2;
    int fifo1_fd, fifo2_fd, fifo1_resp_fd, fifo2_resp_fd;

    fifo1_fd = open(FIFO1, O_RDONLY);
    fifo2_fd = open(FIFO2, O_RDONLY);
    fifo1_resp_fd = open(FIFO1_RESP, O_WRONLY);
    fifo2_resp_fd = open(FIFO2_RESP, O_WRONLY);

    if (fifo1_fd < 0 || fifo2_fd < 0 || fifo1_resp_fd < 0 || fifo2_resp_fd < 0) {
        perror("referee open fifo");
        exit(1);
    }

    while (1) {
        int n1 = read_line(fifo1_fd, buffer1, sizeof(buffer1));
        int n2 = read_line(fifo2_fd, buffer2, sizeof(buffer2));
        if (n1 <= 0 || n2 <= 0) break;

        guess1 = atoi(buffer1);
        guess2 = atoi(buffer2);
        printf("\n");

        int game_over = 0;

        if (guess1 < target) {
            write(fifo1_resp_fd, MSG_HIGHER "\n", strlen(MSG_HIGHER) + 1);
        } 
        else if (guess1 > target) {
            write(fifo1_resp_fd, MSG_LOWER "\n", strlen(MSG_LOWER) + 1);
        } 
        else {
            write(fifo1_resp_fd, MSG_WIN "\n", strlen(MSG_WIN) + 1);
            game_over = 1;
            player1_wins++;
        }
        
        if (guess2 < target) {
            write(fifo2_resp_fd, MSG_HIGHER "\n", strlen(MSG_HIGHER) + 1);
        } 
        else if (guess2 > target) {
            write(fifo2_resp_fd, MSG_LOWER "\n", strlen(MSG_LOWER) + 1);
        } 
        else {
            write(fifo2_resp_fd, MSG_WIN "\n", strlen(MSG_WIN) + 1);
            game_over = 1;
            player2_wins++;
        }
        
        if (game_over) {
            write(fifo1_resp_fd, MSG_GAMEOVER "\n", strlen(MSG_GAMEOVER) + 1);
            write(fifo2_resp_fd, MSG_GAMEOVER "\n", strlen(MSG_GAMEOVER) + 1);
            break;
        }
    }

    close(fifo1_fd);
    close(fifo2_fd);
    close(fifo1_resp_fd);
    close(fifo2_resp_fd);
}

int main() {
    unlink(FIFO1);
    unlink(FIFO1_RESP);
    unlink(FIFO2);
    unlink(FIFO2_RESP);
    
    if (mkfifo(FIFO1, 0666) < 0) {
        perror("mkfifo FIFO1");
        exit(1);
    }
    
    if (mkfifo(FIFO1_RESP, 0666) < 0) {
        perror("mkfifo FIFO1_RESP");
        exit(1);
    }
    
    if (mkfifo(FIFO2, 0666) < 0) {
        perror("mkfifo FIFO2");
        exit(1);
    }
    
    if (mkfifo(FIFO2_RESP, 0666) < 0) {
        perror("mkfifo FIFO2_RESP");
        exit(1);
    }
    
    srand(time(NULL));
    
    for (int game = 1; game <= 10; game++) {
        printf("----- Starting Game %d -----\n", game);
        
        pid_t p1 = fork();
        if (p1 == 0) {
            player1();
        }
        
        pid_t p2 = fork();
        if (p2 == 0) {
            player2();
        }
        
        referee();
        
        wait(NULL);
        wait(NULL);
        
        printf("----- Game %d Results -----\n", game);
        printf("Player 1 Wins: %d\n", player1_wins);
        printf("Player 2 Wins: %d\n\n", player2_wins);
    }
    
    unlink(FIFO1);
    unlink(FIFO1_RESP);
    unlink(FIFO2);
    unlink(FIFO2_RESP);

    printf("----- FINAL TALLY -----\n");
    printf("Player 1 Total Wins: %d\n", player1_wins);
    printf("Player 2 Total Wins: %d\n", player2_wins);
    return 0;
}
