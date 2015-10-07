CC=gcc
CFLAGS=-Wall -ansi
EXEC=lectures

./bin/$(EXEC) : ./obj/$(EXEC).o
	$(CC) -o $@ $^ $(CFLAGS)

./obj/%.o: ./src/semaine2/%.c 
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf ./obj/*.o $(EXEC)