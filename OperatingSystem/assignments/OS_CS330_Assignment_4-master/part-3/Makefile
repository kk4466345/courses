all:serial_hash parallel_hash inputgen
CFLAGS=-I. -g
LDFLAGS=-lpthread

serial_hash:main.c serial_hash.c common.h
	gcc $(CFLAGS) main.c serial_hash.c $(LDFLAGS) -o serial_hash

parallel_hash:main.c parallel_hash.c common.h
	gcc $(CFLAGS) main.c parallel_hash.c $(LDFLAGS) -o parallel_hash

inputgen:inputgen.c common.h
	gcc $(CFLAGS) inputgen.c $(LDFLAGS) -o inputgen
clean:
	rm -f serial_hash parallel_hash inputgen
