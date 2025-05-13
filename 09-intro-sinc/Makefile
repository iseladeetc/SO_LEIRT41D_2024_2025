CC      = gcc

CFLAGS += -Wall 
CFLAGS += -g 
#CFLAGS += -Ofast


BINS = intro_sync intro_sync_mutex intro_sync_semaphore

all: $(BINS)


intro_sync: intro_sync.o
intro_sync.o: intro_sync.c

intro_sync_mutex: intro_sync_mutex.o
intro_sync_mutex.o: intro_sync_mutex.c

intro_sync_semaphore: intro_sync_semaphore.o
intro_sync_semaphore.o: intro_sync_semaphore.c


clean: 
	$(RM) $(BINS) *.o

.PHONY: clean
.PHONY: all
