// 매핑된 메모리를 공유 메모리에 복사하는 함수입니다.
// 매핑된 메모리가 자식과 공유되는 것을 증명하기 위해, 자식을 생성하여 자식이 매핑된 메모리를 공유 메모리에 복사하도록 하였습니다.
// 또한 생성된 자식 프로세스들이 메모리를 복사하기 전까지 실행하지 않도록, 시그널을 사용하였습니다.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>

extern caddr_t randNumsAddr;
extern int randNumsSize;
extern int msgid;
extern int shmid;
extern int *shmaddr;
pid_t *pids;
extern int pidCount;

void mmapToShm() {
    // 공유 메모리 연결
    shmaddr = (int *)shmat(shmid, (char *)NULL, 0);

    int status;

    // 자식을 통해 매핑된 메모리를 공유 메모리에 복사
    pid_t pid = fork();
    switch (pid) {
        case -1:
            perror("fork");
            shmdt(shmaddr);
            munmap(randNumsAddr, randNumsSize);
            exit(1);    
            break;
        case 0:
            // 복사하는 함수로 memcpy 사용
            memcpy(shmaddr, randNumsAddr, randNumsSize);
            munmap(randNumsAddr, randNumsSize);
            exit(0);
            break;
        default:
            // 부모는 자식이 메모리를 복사할 때까지 대기합니다.
            if (waitpid(pid, &status, 0) == -1) {
                perror("waitpid");
                exit(1);
            }
            break;
    }

    sleep(2);

    // SIGSTOP 으로 멈춰있는 생성된 모든 스레드들에게
    // 수행을 시작하라는 시그널을 보냅니다.
    for (int i = 0; i < pidCount; i++) {
        if (kill(pids[i], SIGCONT) == -1) {
            perror("kill");
            shmdt(shmaddr);
            exit(1);
        } 
    }
}