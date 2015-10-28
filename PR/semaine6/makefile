#			Programmation Repartie (4I400)
#		Makefile pour TD 6. La Communication Inter Processus sous POSIX


# Documentation: http://www.gnu.org/software/make/manual/make.html
CC =gcc
LFLAGS = -lpthread -lrt
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
all: $(BIN)remonte_posix_partagee $(BIN)diner_philosophes $(BIN)posix_barrier 
test: test-remonte_posix_partagee test-diner_philosophes test-posix_barrier 

test-remonte_posix_partagee: $(BIN)remonte_posix_partagee
	-bin/remonte_posix_partagee 4

test-posix_barrier: $(BIN)posix_barrier
	-bin/posix_barrier 4

test-diner_philosophes: $(BIN)diner_philosophes
	-bin/diner_philosophes 5 4 2 1


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

