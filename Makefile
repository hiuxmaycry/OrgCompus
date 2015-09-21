CC=gcc
CFLAGS= -Wall -pedantic -std=c99 -g
INPUT= main.c
EXEC= tp1
VFLAGS= valgrind --leak-check=full --track-origins=yes -v --trace-children=yes
VQUIET= -q

all: e r c

e: $(EXEC)
r: run
c: cleanAll
m: memCheck
mq: memCheckQ

%.o : %.c
	$(CC) $(CFLAGS) -c $<

.SILENT:
$(EXEC): $(MIDDLEFILE)
	$(CC) $(CFLAGS) $(INPUT) -o $(EXEC)

.SILENT:
memCheck:
	$(VFLAGS) ./$(EXEC)

.SILENT:
memCheckQ:
	$(VFLAGS) $(VQUIET) ./$(EXEC)

.SILENT:
clean:
	rm $(EXEC)

.SILENT:
cleanAll:
#	rm *.o
	rm $(EXEC)

.SILENT:
run:
	$ ./$(EXEC) 126 60 inicial -o salida