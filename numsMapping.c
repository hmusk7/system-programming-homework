// numsMapping 함수는 메모리 매핑을 통해 파일을 적재하는 함수입니다.

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>


extern int numCount;
extern caddr_t randNumsAddr;
extern int randNumsSize;

void numsMapping() {
    FILE *fp;
    int fd;

    fd = open("randNums.dat", O_RDONLY);
    if (fd == -1) {
        perror("fileno");
        //pclose(fp);
        exit(1);
    }

    // mmap 함수를 통해 첫번째 인자로 NULL 을 줘서 시스템이 적절한 메모리에 적재하도록 하였고,
    // 숫자 값들은 어차피 수정하지 않고 읽기 전용으로만 사용될 것이기 때문에 PROT_READ 와 MAP_PRIVATE 을 인자로 넘겨주었습니다.
    randNumsAddr = mmap(NULL, randNumsSize, PROT_READ, MAP_PRIVATE, fd, (off_t)0);
    if (randNumsAddr == MAP_FAILED){
        perror("mmap");
        exit(1);
    }

    close(fd);
}