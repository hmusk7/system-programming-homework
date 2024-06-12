// 제가 만든 프로그램은 랜덤하게 숫자를 생성해서 파일에 저장하고,
// 그 파일을 불러와 메모리에 매핑하고,
// 매핑된 메모리를 공유 메모리에 복사하고,
// 자식 프로세스들을 생성하여 공유 메모리에 저장된 랜덤한 숫자 값들을 구간을 나눠 더하고,
// 최종적으로 하나로 모아 다 더하는 병렬적인 덧셈 프로그램입니다.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

// 랜덤한 숫자를 생성하여 파일에 저장하는 함수
extern void genRandNums();
// 숫자 파일을 불러와서 메모리에 매핑하는 함수
extern void numsMapping();
// 스레드를 생성해주는 함수
extern void createThreads();
// 매핑된 메모리를 공유 메모리로 복사하는 함수
extern void mmapToShm();
// 최종적으로 공유 메모리에 저장된 값을 불러와 전부 합하는 함수
extern void sumTotal();
// 최종적으로 Makefile 을 통해 생성된 파일 목록을 출력하는 함수
extern void fileList();

int numCount;
caddr_t randNumsAddr;
int randNumsSize;
pid_t *pids;
int pidCount;
// 부모와 자식이 공유할 메시지 큐 식별자, 공유 메모리 식별자
int msgid;
int shmid;
int *shmaddr;

int main() {
    genRandNums();
    numsMapping();
    createThreads();
    mmapToShm();
    sumTotal();
    fileList();


    msgctl(msgid, IPC_RMID, (struct msqid_ds *)NULL);
    shmctl(shmid, IPC_RMID, (struct shmid_ds *)NULL);

    free(pids);
    free(shmaddr);

    return 0;
}