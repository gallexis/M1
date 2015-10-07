#		Makefile pour 4I400 TM 3. Synchronisation de threads


CC = gcc
CFLAGS = -W -Wall -ansi -pedantic
LFLAGS = -pthread

BIN =bin/
OBJ =obj/
INCLUDE =include/
LIB =lib/
SRC =src/


H=


$(BIN)thread_setdetach: $(OBJ)thread_setdetach.o
	$(CC) $(LFLAGS) -o $@ $<

$(BIN)thread_detach: $(OBJ)thread_detach.o
	$(CC) $(LFLAGS) -o $@ $<


$(BIN)thread_broadcast: $(OBJ)thread_broadcast.o
	$(CC) $(LFLAGS) -o $@ $<

$(OBJ)%.o: $(SRC)%.c $H
	$(CC) $(CFLAGS) -o $@ -c $<
	
clean: 
	rm -rf $(OBJ)*.o $(BIN)*

