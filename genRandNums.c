// genRandNums 는 랜덤한 숫자를 생성하는 함수입니다.
// 파일 입출력 함수 fopen, fwrite, fclose
// 파일 정보 검색 함수 stat 이 사용되었습니다.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

extern int numCount;
extern int randNumsSize;

void genRandNums() {
    FILE *fp;
    int *randNums;
    struct stat statbuf;

    printf("랜덤하게 생성할 숫자의 개수를 입력해주세요: ");
    scanf("%d", &numCount);
    printf("\n");
    
    randNumsSize = sizeof(int) * numCount;
    randNums = malloc(randNumsSize);

    // rand 함수를 위한 시드값 설정 (time 함수로 랜덤한 시드 생성)
    srand(time(NULL));

    // 랜덤하게 생성된 숫자를 배열에 하나씩 담습니다.
    for (int i = 0; i < numCount; i++) {
        randNums[i] = rand() % 100;
    }

    // randNums.dat 은 랜덤하게 생성된 숫자를 저장할 파일입니다.
    // fopen 에서 모드를 바이너리 쓰기 모드로, 없으면 생성하도록 만들었습니다.
    fp = fopen("randNums.dat", "wb");
    if (fp == NULL) {
        perror("fopen: randNums.dat");
        exit(1);
    }

    // fwrite 함수를 통해 숫자들을 파일에 출력합니다.
    fwrite(randNums, sizeof(int), numCount, fp);

    // stat 함수를 통해 파일의 모드 정보를 확인합니다.
    stat("randNums.dat", &statbuf);
    printf("randNums.dat 의 st_mode: %o\n", (unsigned int)statbuf.st_mode);

    fclose(fp);
    free(randNums);
}