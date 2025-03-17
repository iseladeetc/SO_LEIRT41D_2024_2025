CC = gcc
CFLAGS = -Wall -g


BINS = fcopy copy

all: $(BINS)

fcopy: fcopy.o
fcopy.o: fcopy.c

copy: copy.o
copy.o: copy.c

clean:
	$(RM) $(BINS) *.o
