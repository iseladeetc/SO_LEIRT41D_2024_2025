CC = gcc
CFLAGS = -Wall -g

BINS = servidor_TCP_iterativo servidor_TCP_concorrente cliente_TCP \
	   servidor_DUnix cliente_DUnix


all: $(BINS)

servidor_TCP_iterativo: servidor_TCP_iterativo.o erroraux.o
servidor_TCP_iterativo.o: servidor_TCP_iterativo.c common.h erroraux.h

servidor_TCP_concorrente: servidor_TCP_concorrente.o erroraux.o
servidor_TCP_concorrente.o: servidor_TCP_concorrente.c common.h erroraux.h

cliente_TCP: cliente_TCP.o erroraux.o
cliente_TCP.o: cliente_TCP.c common.h erroraux.h

servidor_DUnix: servidor_DUnix.o erroraux.o
servidor_DUnix.o: servidor_DUnix.c common.h erroraux.h

cliente_DUnix: cliente_DUnix.o erroraux.o
cliente_DUnix.o: cliente_DUnix.c common.h erroraux.h


erroraux.o: erroraux.c erroraux.h


clean:
	$(RM) $(BINS) *.o
