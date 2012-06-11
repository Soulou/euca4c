CC=gcc
CFLAGS=--std=c99 -I.. -Werror -Wall -g
LDFLAGS=-L../ -leuca4c

TEST1=euca_run_instance
TEST2=euca_terminate_instances

all: $(TEST1) $(TEST2)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $^

$(TEST1): $(TEST1).o
	$(CC) -o $@ $^ $(LDFLAGS)
$(TEST2): $(TEST2).o
	$(CC) -o $@ $^ $(LDFLAGS)

install: $(TEST1) $(TEST2)
	install -m755 $(TEST1) /usr/local/bin 
	install -m755 $(TEST2) /usr/local/bin 

clean:
	rm $(TEST1) $(TEST2) *.o 2>/dev/null || true

uninstall:
	rm /usr/local/bin/$(TEST1)
	rm /usr/local/bin/$(TEST2)
