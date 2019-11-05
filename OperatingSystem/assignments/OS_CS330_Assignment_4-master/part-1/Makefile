all:blockhash
CFLAGS=-I.
LDFLAGS=-lpthread

blockhash:main.c thread_func.c
	gcc $(CFLAGS) main.c thread_func.c $(LDFLAGS) -o blockhash
clean:
	rm -f blockhash
