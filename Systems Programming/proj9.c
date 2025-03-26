#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int player1_guess = -1, player2_guess = -1;
int rdy[2] = {0, 0};  
int random_num;
int player1_score = 0, player2_score = 0;
int p1_cor = 0, p2_cor = 0;
int new_game[2] = {0, 0};  

pthread_mutex_t mtx[2] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};
pthread_cond_t cnd[2] = {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};
pthread_mutex_t referee_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t referee_cond = PTHREAD_COND_INITIALIZER;
volatile int game_over = 0; 

int generate_random(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void* player1_behavior(void* arg) {
    int p1min = 0, p1max = 101;

    while (1) {
        pthread_mutex_lock(&mtx[0]);
        if (game_over) {
            pthread_mutex_unlock(&mtx[0]);
            break;
        }
        if(new_game[0]) {
            p1min = 0;
            p1max = 101;
            new_game[0] = 0;
        }

        rdy[0] = 1;
        pthread_cond_signal(&referee_cond);
        pthread_cond_wait(&cnd[0], &mtx[0]);
        if (game_over) {
            pthread_mutex_unlock(&mtx[0]);
            break;
        }

        player1_guess = (p1min + p1max) / 2;
        printf("Player 1 guessed: %d\n", player1_guess);

        if (player1_guess < random_num) {
            p1min = player1_guess + 1;
        } else if (player1_guess > random_num) {
            p1max = player1_guess - 1;
        } else {
            p1_cor = 1;
        }

        if (p1_cor) {
            p1min = 0;
            p1max = 101;
            usleep(1000000);
            p1_cor = 0;
        }
        else if (p2_cor) {
            p1min = 0;
            p1max = 101;
            usleep(500000);
            p2_cor = 0;
        }
        pthread_mutex_unlock(&mtx[0]);
        usleep(500000);
    }
    return NULL;
}

void* player2_behavior(void* arg) {
    int p2min = 0, p2max = 101;
    srand(time(NULL) ^ (unsigned long)pthread_self());

    while (1) {
        pthread_mutex_lock(&mtx[1]);
        if (game_over) {
            pthread_mutex_unlock(&mtx[1]);
            break;
        }
        if(new_game[1]) {
            p2min = 0;
            p2max = 101;
            new_game[1] = 0;
        }
        rdy[1] = 1;
        pthread_cond_signal(&referee_cond);
        pthread_cond_wait(&cnd[1], &mtx[1]);
        if (game_over) {
            pthread_mutex_unlock(&mtx[1]);
            break;
        }

        if (p2min > p2max) {
            p2min = 0;
            p2max = 101;
        }
        player2_guess = generate_random(p2min, p2max);
        printf("Player 2 guessed: %d\n", player2_guess);

        if (player2_guess < random_num) {
            p2min = player2_guess + 1;
        } else if (player2_guess > random_num) {
            p2max = player2_guess - 1;
        } else {
            p2_cor = 1;
        }

        if (p2_cor) {
            p2min = 0;
            p2max = 101;
            usleep(1000000);
            p2_cor = 0;
        }
        else if (p1_cor) {
            p2min = 0;
            p2max = 101;
            usleep(500000);
            p1_cor = 0;
        }
        pthread_mutex_unlock(&mtx[1]);
        usleep(500000);
    }
    return NULL;
}

void* referee(void* arg) {
    srand(time(NULL));

    for (int game = 1; game <= 10; game++) {
        new_game[0] = 1;
        new_game[1] = 1;

        printf("----- Starting Game %d -----\n", game);

        pthread_mutex_lock(&referee_mtx);
        random_num = generate_random(1, 100);
        printf("Referee chose: %d\n", random_num);

        while (rdy[0] == 0 || rdy[1] == 0) {
            pthread_cond_wait(&referee_cond, &referee_mtx);
        }
        rdy[0] = rdy[1] = 0;
        pthread_mutex_unlock(&referee_mtx);

        while (1) {
            pthread_mutex_lock(&mtx[0]);
            pthread_cond_signal(&cnd[0]);
            pthread_mutex_unlock(&mtx[0]);

            pthread_mutex_lock(&mtx[1]);
            pthread_cond_signal(&cnd[1]);
            pthread_mutex_unlock(&mtx[1]);

            if (player1_guess == random_num) {
                player1_score++;
                printf("Player 1 guessed correctly!\n");
                break;
            }
            if (player2_guess == random_num) {
                player2_score++;
                printf("Player 2 guessed correctly!\n");
                break;
            }
            usleep(100000);  
        }
        player1_guess = player2_guess = -1;
        usleep(500000); 
    }

    game_over = 1;
    pthread_mutex_lock(&mtx[0]);
    pthread_cond_signal(&cnd[0]);
    pthread_mutex_unlock(&mtx[0]);

    pthread_mutex_lock(&mtx[1]);
    pthread_cond_signal(&cnd[1]);
    pthread_mutex_unlock(&mtx[1]);

    printf("----- FINAL TALLY -----\n");
    printf("Player 1 Total Wins: %d\n", player1_score);
    printf("Player 2 Total Wins: %d\n", player2_score);

    return NULL;
}

int main() {
    pthread_t player1_thread, player2_thread, referee_thread;

    pthread_create(&player1_thread, NULL, player1_behavior, NULL);
    pthread_create(&player2_thread, NULL, player2_behavior, NULL);
    pthread_create(&referee_thread, NULL, referee, NULL);

    pthread_join(referee_thread, NULL);
    pthread_join(player1_thread, NULL);
    pthread_join(player2_thread, NULL);

    return 0;
}
