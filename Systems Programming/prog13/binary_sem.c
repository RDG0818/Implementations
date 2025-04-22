#include <sys/types.h>
#include <sys/sem.h>
#include "semun.h"
#include "binary_sem.h"
#include <stdio.h>
#include <stdlib.h>

int bsUseSemUndo = 0;
int bsRetryOnEintr = 1;


int initSemAvailable(int semId, int semNum)
{
    union semun arg;
    arg.val = 1;
    return semctl(semId, semNum, SETVAL, arg);
}

int initSemInUse(int semId, int semNum)
{
    union semun arg;
    arg.val = 0;
    return semctl(semId, semNum, SETVAL, arg);
}

int reserveSem(int semId, int semNum)
{
    struct sembuf sops;

    sops.sem_num = semNum;
    sops.sem_op = -1;
    sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

    while (semop(semId, &sops, 1) == -1)
    {
        if (errno != EINTR || !bsRetryOnEintr)
            return -1;
    }

    return 0;
}

int releaseSem(int semId, int semNum)
{
    struct sembuf sops;

    sops.sem_num = semNum;
    sops.sem_op = 1;
    sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

    return semop(semId, &sops, 1);
}


#define MAX_BLOCKS 50
typedef struct {
    int length;
    char character;
} Block;

typedef struct {
    int total_blocks;
    Block blocks[MAX_BLOCKS];
} ImageData;


void generateImage(ImageData *data) {
    int width = 6;
    int current_width = 0;
    int block_index = 0;

    for (int i = 0; i < data->total_blocks; ++i) {
        for (int j = 0; j < data->blocks[i].length; ++j) {
            putchar(data->blocks[i].character);
            current_width++;
            if (current_width == width) {
                putchar('\n');
                current_width = 0;
            }
        }
    }
    if (current_width > 0) putchar('\n');

}
