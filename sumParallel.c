// 병렬적으로 구간별로 나눠 합을 수행하는 함수입니다.
// 처리가 끝나면 메시지 큐에 처리가 끝났다는 메시지를 업데이트하고, 종료합니다.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>


struct msgbuf {
    long mtype;
    char mtext[100];
};

int main (int argc, char *argv[]) {
    // 부모가 mmapToShm 을 전부 완료하여 매핑된 메모리를 공유 메모리로 전부 복사했을 때 자식이 병렬적으로 프로그래밍을 수행하도록,
    // 시그널을 통해 관리
    kill(getpid(), SIGSTOP);


    // 여기부터는 병렬로 덧셈을 처리하는 부분입니다.
    int argvint[5];
    int msgid;
    struct msgbuf msg;
    int shmid;
    int *shmaddr;
    int numCount;
    int pidCount;
    int index;

    int part;
    int remainder;
    int sum = 0;

    for (int i = 1; i < argc - 1; i++) {
        argvint[i-1] = atoi(argv[i]);
    }

    msgid = argvint[0];
    shmid = argvint[1];
    numCount = argvint[2];
    pidCount = argvint[3];
    index = argvint[4];

    shmaddr = (int *)shmat(shmid, (char *)NULL, 0);
    
    part = numCount / pidCount;
    remainder = (numCount % pidCount);

    if (index == pidCount - 1) {
        for (int i = part * index; i < (part * (index + 1)) + remainder; ++i) {
            sum += shmaddr[i];
        }
    } else {
        for (int i = part * index; i < part * (index + 1); ++i) {
            sum += shmaddr[i];
        }
    }

    // 전부 계산하면 공유 메모리의 특정한 위치에 결과값을 올립니다.
    shmaddr[part * index] = sum;

    // 처리가 끝나면 메시지 큐에 처리가 끝났다는 메시지를 전송합니다.
    msg.mtype = 1;
    strcpy(msg.mtext, "자식 프로세스 부분합 완료");
    if (msgsnd(msgid, (void *)&msg, 100, IPC_NOWAIT) == -1) {
        perror("msgsnd");
        exit(1);
    }

    shmdt(shmaddr);
    free(shmaddr);

    return 0;
}