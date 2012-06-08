CC=gcc
CFLAGS=-I/usr/include/python2.7 -g
LDFLAGS=-lpython2.7
EXEC=testmain

all: $(EXEC)

.c.o:
	$(CC) $(CFLAGS) $^ -c -o $@

$(EXEC): euca_binding.o
	$(CC) $^ $(LDFLAGS) -o $@ 
