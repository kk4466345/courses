all:rwlock
CFLAGS=-I. -g
LDFLAGS=-lpthread

rwlock:main.c rwlock.c common.h
	gcc $(CFLAGS) main.c rwlock.c $(LDFLAGS) -o rwlock
clean:
	rm -f rwlock
