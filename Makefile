# Makefile

CC=gcc
CFLAGS=
OBJS=main.o genRandNums.o numsMapping.o createThreads.o mmapToShm.o sumTotal.o fileList.o
LIBS=

all: main.out sumParallel.out

main.out: $(OBJS)
	$(CC) $(CFLAGS) -o main.out $(OBJS) $(LIBS)
sumParallel.out: sumParallel.o
	$(CC) $(CFLAGS) -o sumParallel.out sumParallel.o $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
genRandNums.o: genRandNums.c
	$(CC) $(CFLAGS) -c genRandNums.c
numsMapping.o: numsMapping.c
	$(CC) $(CFLAGS) -c numsMapping.c
createThreads.o: createThreads.c
	$(CC) $(CFLAGS) -c createThreads.c
mmapToShm.o: mmapToShm.c
	$(CC) $(CFLAGS) -c mmapToShm.c
sumTotal.o: sumTotal.c
	$(CC) $(CFLAGS) -c sumTotal.c
sumParallel.o: sumParallel.c
	$(CC) $(CFLAGS) -c sumParallel.c
fileList.o: fileList.c
	$(CC) $(CFLAGS) -c fileList.c

clean:
	rm -f $(OBJS) main.out sumParallel.out *.o randNums.dat core
