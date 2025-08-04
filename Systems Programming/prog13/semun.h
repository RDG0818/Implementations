#ifndef SEMUN_H
#define SEMUN_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int              val;    
    struct semid_ds *buf;    
    unsigned short  *array;  
    struct seminfo  *__buf;  
};

#endif // SEMUN_H