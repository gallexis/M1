CC=gcc
CFLAGS=-Wall -ansi
EXEC=nfork

./bin/$(EXEC) : ./obj/$(EXEC).o
	$(CC) -o $@ $^ $(CFLAGS)

./obj/%.o: ./src/semaine1/%.c 
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf ./obj/*.o $(EXEC)