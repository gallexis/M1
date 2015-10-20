#			Programmation Repartie (4I400)
#		Makefile pour TD 4. Processus légers et fichiers


CC =gcc
LFLAGS = -lpthread
CFLAGS =-W -Wall -ansi -pedantic -Iinclude

BIN =bin/
OBJ =obj/
INCLUDE =include/
LIB =lib/
SRC =src/

H=include/producteur_consommateur.h 
O=src/upper.o

all: $(BIN)thread_upper $(BIN)thread_uppermax $(BIN)producteur_consommateur $(BIN)producteurs_consommateurs

test: test-thread_upper test-thread_uppermax test-producteur_consommateur test-producteurs_consommateurs

test-thread_upper: $(BIN)thread_upper
	bin/thread_upper fichier1 fichier2 fichier3 fichier4

test-thread_uppermax: $(BIN)thread_uppermax
	bin/thread_uppermax 2 fichier1 fichier2 fichier3

test-producteur_consommateur: $(BIN)producteur_consommateur
	echo "123456789" | bin/producteur_consommateur

test-producteurs_consommateurs: $(BIN)producteurs_consommateurs
	echo "123456789" | bin/producteurs_consommateurs 5 3


$(BIN)%: $(OBJ)%.o 
	@if [ -d $(BIN) ]; then : ; else mkdir $(BIN); fi
	$(CC) $(LFLAGS) -o $@ $<

$(OBJ)%.o: $(SRC)%.c $H
	@if [ -d $(OBJ) ]; then : ; else mkdir $(OBJ); fi
	$(CC) $(CFLAGS) -o $@ -c $<

clean: 
	rm -rf $(OBJ)*.o $(BIN)*
