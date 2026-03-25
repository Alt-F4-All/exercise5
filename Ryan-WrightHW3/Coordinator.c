#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define CHILDREN 4

int main(int argc, char *argv[]){
    if(argc != 6){
        exit(1);
    }

    int divisor = atoi(argv[1]);

    pid_t pids[CHILDREN];
    int shm_ids[CHILDREN];
    int dividers[CHILDREN];

    for(int i = 0; i < CHILDREN; i++){
        dividers[i] = atoi(argv[i+2]);
    }

    for(int i = 0; i < CHILDREN; i++){
        int pipefd[2];
        pipe(pipefd);

        shm_ids[i] = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
        if(shm_ids[i] < 0){
            exit(1);
        }
    

        pids[i] = fork();

        if(pids[i] < 0){
            exit(1);
        }

        if(pids[i] == 0){
            close(pipefd[1]);

            char divisor_str[20], dividened_str[20], fd_str[20];

            sprintf(divisor_str, "%d", divisor);
            sprintf(dividened_str, "%d", dividers[i]);
            sprintf(fd_str, "%d", pipefd[0]);

            execl("./checker", "checker", divisor_str, dividened_str, fd_str, NULL);
            exit(1);
        }else{
            printf("Coordinator: forked process with ID %d.\n", pids[i]);
            close(pipefd[0]);
            write(pipefd[1], &shm_ids[i], sizeof(int));
            printf("Coordinator: wrote shm ID %d to pipe (4 bytes)\n", shm_ids[i]);
            close(pipefd[1]);
        }
    }
    for(int i = 0; i < CHILDREN; i++){
        printf("Coordinator: waiting on child process ID %d...\n", pids[i]);
        waitpid(pids[i], NULL, 0);
        int *result = (int *)shmat(shm_ids[i], NULL, 0);
        if(*result == 1){
            printf("Coordinator: result 1 read from shared memory: %d is divisible by %d.\n", dividers[i], divisor);
        }else{
            printf("Coordinator: result 0 read from shared memory: %d is not divisible by %d.\n", dividers[i], divisor);
        }
        shmdt(result);
        shmctl(shm_ids[i], IPC_RMID, NULL);
    }
    printf("Coordinator: exiting.\n");
    return 0;
}