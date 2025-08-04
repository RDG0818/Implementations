#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define SIG_MYRT1 (SIGRTMIN + 0)
#define SIG_MYRT2 (SIGRTMIN + 1)
#define SIG_GSLOW (SIGRTMIN + 2)
#define SIG_GHIGH (SIGRTMIN + 3)
#define SIG_WIN (SIGRTMIN + 4)
#define SIG_GSLOW2 (SIGRTMIN + 5)
#define SIG_GHIGH2 (SIGRTMIN + 6)
#define SIG_WIN2 (SIGRTMIN + 7)
#define SIG_GAMEEND (SIGRTMIN + 8)

volatile sig_atomic_t player1_ready = 0;
volatile sig_atomic_t player2_ready = 0;
volatile sig_atomic_t player1_guess_signal = 0;
volatile sig_atomic_t player2_guess_signal = 0;
pid_t player1_pid, player2_pid;
int terminate_flag = 0;
int guess_low = 0;
int guess_high = 0;
int guess_low_p2 = 0;
int guess_high_p2 = 0;
int player1_game_end = 0;
int player2_game_end = 0;
int player1_wins = 0;
int player2_wins = 0;
int game_end = 0;

int generate_random(int min, int max) {
    return (rand() % (max - min)) + min;
}

// Parent Signals

void handle_sigusr1(int sig) {
    player1_ready = 1;
    guess_low = guess_high = player1_game_end = 0;
}

void handle_sigmyrt1(int sig) {
    player1_guess_signal = 1;
}

void handle_sigusr2(int sig) {
    player2_ready = 1;
}

void handle_sigmyrt2(int sig) {
    player2_guess_signal = 1;
}

void handle_sigint(int sig) {
    printf("\nTerminating game...\n");

    if (player1_pid > 0) {
        kill(player1_pid, SIGTERM); 
    }
    if (player2_pid > 0) {
        kill(player2_pid, SIGTERM); 
    }

    waitpid(player1_pid, NULL, 0);
    waitpid(player2_pid, NULL, 0);

    exit(0);
}

// Child Signals

void handle_sigterm(int sig) {
    terminate_flag = 1;
}

void handle_game_end(int sig) {
    game_end = 1;
}

void handle_guess_low(int sig) {
    guess_low = 1;
}

void handle_guess_high(int sig) {
    guess_high = 1;
}

void handle_sig_p1_game_end(int sig) {
    player1_game_end = 1;
}

void handle_sig_p2_game_end(int sig) {
    player2_game_end = 1;
}

void player1() {
    struct timespec ts = {0, 50000000};
    struct sigaction sa;

    sa.sa_handler = handle_sigterm;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);

    sa.sa_handler = handle_sig_p1_game_end;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_WIN, &sa, NULL);

    sa.sa_handler = handle_sig_p2_game_end;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_WIN2, &sa, NULL);

    sa.sa_handler = handle_guess_low;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_GSLOW, &sa, NULL);

    sa.sa_handler = handle_guess_high;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_GHIGH, &sa, NULL);

    int min = 0;
    int max = 100;
    int guess = 0;
    char buf[32];

    while (!terminate_flag) {
        kill(getppid(), SIGUSR1);
        guess_low = guess_high = 0;

        guess = (min + max)/2;

        int fd = open("player1_guess.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);

        if (fd < 0) {
            perror("open player1_guess.txt");
            exit(1);
        }

        int len = snprintf(buf, sizeof(buf), "%d\n", guess);
        write(fd, buf, len);
        close(fd);
        
        kill(getppid(), SIG_MYRT1);
        printf("Player 1 Guess: %d\n", guess);

        while(!guess_low && !guess_high && !player1_game_end) {
            pause();
            nanosleep(&ts, NULL);
        }

        if (player2_game_end) {
            exit(0);
        }

        if (guess_low) {
            min = guess;
        }
        else if (guess_high) {
            max = guess;
        }
        else {
            printf("Player 1 Wins!\n");
            exit(0);
        }

        sleep(1);
    }

    if (terminate_flag) exit(0);
}

// Child Player2 Signals

void handle_guess_low_p2(int sig) {
    guess_low_p2 = 1;
}

void handle_guess_high_p2(int sig) {
    guess_high_p2 = 1;
}

void player2() {
    struct timespec ts = {0, 50000000};
    struct sigaction sa;

    sa.sa_handler = handle_sigterm;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);

    sa.sa_handler = handle_guess_low_p2;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_GSLOW2, &sa, NULL);

    sa.sa_handler = handle_guess_high_p2;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_GHIGH2, &sa, NULL);

    sa.sa_handler = handle_sig_p1_game_end;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_WIN, &sa, NULL);

    sa.sa_handler = handle_sig_p2_game_end;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_WIN2, &sa, NULL);

    int min = 0;
    int max = 101;
    int guess = 0;
    char buf[32];

    while (!terminate_flag) {
        kill(getppid(), SIGUSR2);
        guess_low_p2 = guess_high_p2 = 0;

        guess = generate_random(min, max);

        int fd = open("player2_guess.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);

        if (fd < 0) {
            perror("open player2_guess.txt");
            exit(1);
        }

        int len = snprintf(buf, sizeof(buf), "%d\n", guess);
        write(fd, buf, len);
        close(fd);
        
        kill(getppid(), SIG_MYRT2);
        printf("Player 2 Guess: %d\n", guess);

        while(!guess_low_p2 && !guess_high_p2 && !player2_game_end) {
            pause();
            nanosleep(&ts, NULL);
        }

        if (player1_game_end) {
            exit(0);
        }

        if (guess_low_p2) {
            min = guess;
        }
        else if (guess_high_p2) {
            max = guess;
        }
        else {
            printf("Player 2 Wins!\n");
            exit(0);
        }

        sleep(1);
    }

    if (terminate_flag) exit(0);
}

void referee() {
    struct sigaction sa;
    
    sa.sa_handler = handle_sigusr1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_handler = handle_sigmyrt1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_MYRT1, &sa, NULL);

    sa.sa_handler = handle_sigusr2;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR2, &sa, NULL);

    sa.sa_handler = handle_sigmyrt2;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_MYRT2, &sa, NULL);

    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    srand(time(NULL));
    int random_num = generate_random(1, 101);

    printf("Random Number: %d\n", random_num);

    struct timespec ts = {1, 0}; 

    while(!player1_game_end && !player2_game_end) {
        
        while (!player1_ready || !player2_ready) {
            //printf("Waiting for players to be ready... Player 1: %d, Player 2: %d\n", player1_ready, player2_ready);
            nanosleep(&ts, NULL);
        }
    
        //printf("Both players are ready.\n");
    
        player1_ready = player2_ready = 0;
    
        while (!player1_guess_signal || !player2_guess_signal) {
            //printf("Waiting for players to guess... Player 1: %d, Player 2: %d\n", player1_guess_signal, player2_guess_signal);
            nanosleep(&ts, NULL);
        }
    
        //printf("Both players have guessed.\n");
    
        player1_guess_signal = player2_guess_signal = 0;
    
        int player1_fd = open("player1_guess.txt", O_RDWR | O_CREAT, 0644);
        int player2_fd = open("player2_guess.txt", O_RDWR | O_CREAT, 0644);
    
        char buf[32];
        char buf2[32];
        ssize_t bytes = read(player1_fd, buf, sizeof(buf)-1);
        ssize_t bytes2 = read(player2_fd, buf2, sizeof(buf2-1));
        if (bytes < 0) {
            perror("read p1");
            close(player1_fd);
            return;
        }
        if (bytes2 < 0) {
            perror("read p2");
            close(player2_fd);
            return;
        }
    
        buf[bytes] = '\0';
        buf2[bytes2] = '\0';
    
        int player1_guess = atoi(buf);
        int player2_guess = atoi(buf2);
    
        if (player1_guess < random_num) {
            kill(player1_pid, SIG_GSLOW);
        }
        else if (player1_guess > random_num) {
            kill(player1_pid, SIG_GHIGH);
        }
        else {
            player1_wins++;
            kill(player1_pid, SIG_WIN);
            kill(player2_pid, SIG_WIN);
            player1_game_end = 1;
        }

        if (player2_guess < random_num) {
            kill(player2_pid, SIG_GSLOW2);
        }
        else if (player2_guess > random_num) {
            kill(player2_pid, SIG_GHIGH2);
        }
        else {
            player2_wins++;
            kill(player1_pid, SIG_WIN2);
            kill(player2_pid, SIG_WIN2);
            player2_game_end = 1;
        }
        
        close(player1_fd);
        close(player2_fd);
        printf("\n");
    }

}

int main() {

    for (int game = 1; game <= 10; game++) {
        printf("----- Starting Game %d -----\n", game);

        player1_game_end = 0;
        player2_game_end = 0;

        if ((player1_pid = fork()) == 0) {
            int fd = open("player1_guess.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
            close(fd);
            player1();
            exit(0);
        }

        if ((player2_pid = fork()) == 0) {
            int fd = open("player2_guess.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
            close(fd);
            player2();
            exit(0);
        }

        referee();

        wait(NULL);
        wait(NULL);

        printf("----- Game %d Results -----\n", game);
        printf("Player 1 Running Wins: %d\n", player1_wins);
        printf("Player 2 Running Wins: %d\n\n", player2_wins);
    }

    printf("----- FINAL TALLY -----\n");
    printf("Player 1 Total Wins: %d\n", player1_wins);
    printf("Player 2 Total Wins: %d\n", player2_wins);
    return 0;
}