// 모든 스레드가 병렬적으로 부분합 연산을 끝내고 나면,
// 최종적으로 모든 부분합을 더해 전체합을 구하는 함수입니다.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>


extern int numCount;
extern int pidCount;
extern int msgid;
extern int shmid;
extern char *shmaddr;

struct msgbuf {
    long mtype;
    char mtext[100];
};

void sumTotal() {
    int status;
    struct msgbuf msg;

    // 자식들이 모두 종료될 때까지 대기
    // -1 이 리턴되면 살아있는 자식 프로세스가 존재하지 않음
    while (wait(&status) != -1) {
        msgrcv(msgid, &msg, 100, 0, 0);
        // 자식으로부터 받은 메시지를 출력합니다. (메시지 큐에 업데이트 된 메시지들)
        printf("자식으로부터 받은 메시지: %s\n", msg.mtext);
        fflush(stdout);
    }
    
    int sum = 0;
    int part = numCount / pidCount;
    for (int i = 0; i < pidCount; i++) {
        sum += shmaddr[part * i];
    }

    printf("랜덤하게 생성된 숫자들의 합계: %d\n", sum);
}