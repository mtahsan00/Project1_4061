CFLAGS = -Wall -g
CC = gcc $(CFLAGS)

commando_main : commando.o cmd.o cmdcol.o util.o commando.h
	$(CC) -o commando commando.o cmd.o cmdcol.o util.o
	@echo commando is ready

commando.o : commando.c commando.h
	$(CC) -c commando.c

cmd.o : cmd.c commando.h
	$(CC) -c cmd.c

cmdcol.o : cmdcol.c commando.h
	$(CC) -c cmdcol.c

util.o : util.c commando.h
	$(CC) -c util.c

clean :
		@echo Cleaning up object files
		rm -f *.o commando
include test_Makefile		# add at the END of Makefile
