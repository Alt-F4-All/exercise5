#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main(int argc, char *argv[]){
    if(argc != 4){
        exit(1);
    }

    int divisor = atoi(argv[1]);
    int dividend = atoi(argv[2]);
    int pipefd = atoi(argv[3]);

    printf("Checker: process [%d]: starting. \n", getpid());

    int shm_id;
    int bytes = read(pipefd, &shm_id, sizeof(int));

    printf("Checker: process [%d]: read %d bytes containing shm ID %d\n", getpid(), bytes, shm_id);
    int *shared_memory = (int *)shmat(shm_id, NULL, 0);

    if(dividend % divisor == 0){
        printf("Checker process [%d]: %d *IS* divisible by %d.\n", getpid(), dividend, divisor);
        *shared_memory = 1;
    }else{
        printf("Checker process [%d]: %d *IS NOT* divisible by %d.\n", getpid(), dividend, divisor);
        *shared_memory = 0;
    }
    shmdt(shared_memory);
    close(pipefd);
    return 0;
}