CC=gcc
CFLAGS=-Wall -ansi
EXEC=forkfilemax

./bin/$(EXEC) : ./obj/$(EXEC).o
	$(CC) -o $@ $^ $(CFLAGS)

./obj/%.o: ./src/%.c 
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf ./obj/*.o $(EXEC)