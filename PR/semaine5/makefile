#			Programmation Repartie (4I400)
#		Makefile pour TD 5. Introduction à la communication inter-processus (IPC)


# Documentation: http://www.gnu.org/software/make/manual/make.html
CC =gcc
LFLAGS =
CFLAGS =-W -Wall -ansi -pedantic -Iinclude

BIN =bin/
OBJ =obj/
INCLUDE =include/
LIB =lib/
SRC =src/

.SUFFIXES:

HC=
O=


.PHONY: all clean test
all: $(BIN)remonte_ipc $(BIN)remonte_partagee $(BIN)multi_remonte $(BIN)multi_remonte_file_unique
test: test-remonte_ipc test-remonte_partagee test-multi_remonte test-multi_remonte_file_unique

test-remonte_ipc: $(BIN)remonte_ipc
	-bin/remonte_ipc 4

test-remonte_partagee: $(BIN)remonte_partagee
	-bin/remonte_partagee 4

test-multi_remonte: $(BIN)multi_remonte
	-bin/multi_remonte 4

test-multi_remonte_file_unique: $(BIN)multi_remonte_file_unique
	-bin/multi_remonte_file_unique 4


clean: 
	rm -rf $(OBJ)*.o $(BIN)*

$(BIN)%: $(OBJ)%.o $O
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) $(LFLAGS) -o $@ $^

$(OBJ)%.o: $(SRC)%.c $(HC)
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

$(INCLUDE)%.h:
	@if [ -d $(INCLUDE) ]; then : ; else mkdir $(INCLUDE); fi

