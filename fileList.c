// Makefile 을 통해 생성된 파일들을 출력합니다.
// 파이프의 popen 함수를 통해 간단하게 읽기 전용으로 셸 명령어를 얻어와 출력하도록 하였습니다.

#include <stdlib.h>
#include <stdio.h>

void fileList() {
    FILE *fp;
    char buf[1024];
    
    // popen 을 통해 ls 명령을 읽기 전용으로 얻어옵니다.
    fp = popen("ls", "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    printf("생성된 파일 목록: ");
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        printf("%s", buf);
    }

    pclose(fp);
}