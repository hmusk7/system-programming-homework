// 병렬적으로 덧셈을 실행할 프로세스를 생성하는 함수입니다.
// 프로세스 생성 여부 검사에서는 시스테 정보가 사용됐습니다.
// 또한 시스템 V IPC 인 메시지 큐, 공유 메모리가 사용되었고,
// fork() 및 execl 함수를 통해 자식 프로세스를 생성했습니다.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>

extern int numCount;
extern caddr_t randNumsAddr;
extern pid_t *pids;
extern int pidCount;
extern int msgid;
extern int shmid;


// 프로세스를 생성할 수 있는지 검사합니다.
void checkProcess() {
    int vProcNums;
    struct sysinfo info;

    sysinfo(&info);
    // 넉넉잡아 실행 가능한 프로세스의 개수
    vProcNums = sysconf(_SC_CHILD_MAX) - info.procs;

    printf("생성할 스레드의 개수를 입력하세요: ");
    scanf("%d", &pidCount);
    printf("\n");

    if (pidCount > vProcNums) {
        printf("생성 가능한 스레드의 개수를 초과했습니다.");
        exit(1);
    } else if (pidCount > numCount) {
        printf("생성된 숫자의 개수보다 스레드 개수가 많습니다.");
        exit(1);
    }

    pids = malloc(sizeof(pid_t) * pidCount);
}

void createThreads() {

    checkProcess();

    // 각 프로세스들에 인자로 넘겨줄 값을 저장할 배열들입니다.
    char msgidStr[12];
    char shmidStr[12];
    char numCountStr[12];
    char pidCountStr[4];
    char index[4];

    // 메시지 큐 식별자 생성
    // IPC_PRIVATE 으로 설정했기 때문에, 식별자 값을 인자로 넘겨줌으로써 execl 함수를 통해 프로그램이 새로 적재되더라도,
    // 메시지 큐에 접근할 수 있도록 했습니다.
    msgid = msgget(IPC_PRIVATE, IPC_CREAT | 0644);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // 공유 메모리 식별자 생성
    // 메시지 큐와 마찬가지로 자식 프로세스에 인자로 넘겨줄 공유 메모리 식별자입니다.
    shmid = shmget(IPC_PRIVATE, 1000000, IPC_CREAT | 0644);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // 자식 프로세스 execl 함수에 인자로 메시지 큐 식별자와 공유 메모리 식별자를 보내기 위해 문자열 형태로 변환합니다.
    // 이 외에도 병렬적으로 처리하기 위해 범위를 계산하기 위한 값들을 문자열 형태로 변환합니다.
    snprintf(msgidStr, sizeof(msgidStr), "%d", msgid);
    snprintf(shmidStr, sizeof(shmidStr), "%d", shmid);
    snprintf(numCountStr, sizeof(numCountStr), "%d", numCount);
    snprintf(pidCountStr, sizeof(pidCountStr), "%d", pidCount);
    

    // 자식 프로세스 생성하며 execl 함수를 통해 sumParallel 프로그램을 적재
    for (int i = 0; i < pidCount; i++) {
        switch (pids[i] = fork()) {
            case -1:
                perror("fork failed");
                exit(EXIT_FAILURE);
            case 0:
                snprintf(index, sizeof(index), "%d", i);
                printf("자식 프로세스 생성됨\n");
                fflush(stdout);
                if (execl("./sumParallel.out", "sumParallel", msgidStr, shmidStr, numCountStr, pidCountStr, index, (char *)NULL) == -1) {
                    perror("execl");
                    exit(1);
                }
                exit(0);
            default:
                printf("부모 프로세스는 실행중\n");
                fflush(stdout);
        }
    }
}