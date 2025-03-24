CC = gcc
CFLAGS = -Wall -g


BINS = redirectStdout getStdoutCmd pipeLineExec

AUX_FILES = out.txt


all: $(BINS)

redirectStdout: redirectStdout.o
redirectStdout.o: redirectStdout.c

getStdoutCmd: getStdoutCmd.o
getStdoutCmd.o: getStdoutCmd.c

pipeLineExec: pipeLineExec.o
pipeLineExec.o: pipeLineExec.c


clean:
	$(RM) $(BINS) *.o $(AUX_FILES)
