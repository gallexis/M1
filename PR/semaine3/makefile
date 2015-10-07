#		Makefile pour 4I400 TD 3. Introduction au processus légers


CC = gcc
CFLAGS = -W -Wall -ansi -pedantic
LFLAGS = -lpthread

BIN =bin/
OBJ =obj/
INCLUDE =include/
LIB =lib/
SRC =src/


H=

$(BIN)thread_wait: $(OBJ)thread_wait.o
	$(CC) $(LFLAGS) -o $@ $<

$(BIN)thread_rand: $(OBJ)thread_rand.o
	$(CC) $(LFLAGS) -o $@ $<
	
	
$(BIN)thread_create: $(OBJ)thread_create.o
	$(CC) $(LFLAGS) -o $@ $<



$(OBJ)%.o: $(SRC)%.c $H
	$(CC) $(CFLAGS) -o $@ -c $<
	
clean: 
	rm -rf $(OBJ)*.o $(BIN)*
