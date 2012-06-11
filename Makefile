CC=gcc
CFLAGS=--std=c99 -Werror -Wall -g
LDFLAGS=-lpython2.7
LIB=libeuca4c.so

all: $(LIB)

tests:
	$(MAKE) -C tests/

euca4c.o: euca4c.c
	$(CC) -fPIC $(CFLAGS) $^ -c -o $@

$(LIB): euca4c.o
	$(CC) -shared -Wl $< $(LDFLAGS) -o $@ 

clean:
	$(MAKE) -C tests/ clean
	rm $(LIB) *.o *.pyc 2>/dev/null || true
	@echo "Cleaning data"

install: $(LIB)
	install -m644 $(LIB) /usr/local/lib 
	install -m644 euca_run_instance.py /usr/local/lib/python2.7/dist-packages/ 
	install -m644 euca_terminate_instances.py /usr/local/lib/python2.7/dist-packages/ 
	install -m644 euca_describe_instances.py /usr/local/lib/python2.7/dist-packages/ 
	install -m644 euca4c.h /usr/local/include/ 

install_all: install 
	$(MAKE) -C tests/ install

uninstall:
	$(MAKE) -C tests/ uninstall
	rm /usr/local/lib/$(LIB)
	rm /usr/local/lib/python2.7/euca_run_instance.py 
	rm /usr/local/lib/python2.7/euca_terminate_instances.py 
	rm /usr/local/lib/python2.7/euca_describe_instances.py 
	rm /usr/local/include/euca4c.h
