CC = gcc
CFLAGS = -Wall -g

BINS = fifoServer fifoClient

all: $(BINS)

fifoServer: fifoServer.o
fifoServer.o: fifoServer.c common.h

fifoClient: fifoClient.o
fifoClient.o: fifoClient.c common.h


clean:
	$(RM) $(BINS) *.o
