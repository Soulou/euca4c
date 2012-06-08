CC=gcc
CFLAGS=--std=c99 -Werror -Wall -g
LDFLAGS=-lpython2.7
CTESTFLAGS=--std=c99 -I. -Werror -Wall -g
LDTESTFLAGS=-L. -leuca4c
LIB=libeuca4c.so
TEST_EXEC=euca_run_instance

all: $(LIB)

test: $(TEST_EXEC)

.c.o:
	$(CC) -fPIC $(CFLAGS) $^ -c -o $@

$(LIB): euca4c.o
	$(CC) -shared -Wl $^ $(LDFLAGS) -o $@ 

$(TEST_EXEC): $(TEST_EXEC).c
	$(CC) $(CTESTFLAGS) -c $^ -o $@.o
	$(CC) $@.o -o $@ $(LDTESTFLAGS) 

clean:
	@rm $(LIB) $(TEST_EXEC) *.o *.pyc 2>/dev/null || true
	@echo "Cleaning data"

install: $(LIB)
	install -m644 $(LIB) /usr/local/lib 
	install -m644 euca_run_instance.py /usr/local/lib/python2.7/dist-packages/ 
	install -m644 euca4c.h /usr/local/include/ 

install_test: install $(LIB)
	install -m755 $(TEST_EXEC) /usr/local/bin 

install_all: install install_test 

uninstall:
	rm /usr/local/lib/$(LIB)
	rm /usr/local/bin/$(TEST_EXEC)
	rm /usr/local/lib/python2.7/euca_run_instance.py 
	rm /usr/local/include/euca4c.h
