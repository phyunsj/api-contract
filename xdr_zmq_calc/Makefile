PACKAGE= xdr-tests

CC= gcc
CFLAGS= -g -Wall -Wno-unused -Werror -Wdeprecated -Wformat
LFLAGS= -lzmq

all: calc calc_server calc_client

calc_server: calc_xdr.o calc_server.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

calc_client: calc_xdr.o calc_client.o 
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

calc: calc.x
	rm -f $@
	rpcgen  $<

clean:
	rm -f calc_server calc_client calc.h calc_xdr.c *.o core *~ 