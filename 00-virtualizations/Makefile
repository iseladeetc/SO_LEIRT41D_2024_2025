CC 		= gcc
CFLAGS += -Wall
CFLAGS += -g
LDLIBS  = -lm

BINS = cpu_stress vmem

all: $(BINS)

cpu_stress: cpu_stress.o
cpu_stress.o: cpu_stress.c

vmem: vmem.o
vmem.o: vmem.c

# ASLR (Address Space Layout Randomization)
# Esta característica está activa por omissão nos sistemas Linux
# atribui de forma aleatória o endereço base das zonas de memória do processo,
# como, por exemplo, stack, data, heap, code, libs
#
# podemos desativar apenas para a execução de um programa 
#	setarch --addr-no-randomize ./vmem
# ou
#	setarch -R ./vmem
run_vmem:
	setarch --addr-no-randomize ./vmem

clean:
	$(RM) $(BINS) *.o