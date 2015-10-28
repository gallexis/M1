CC     = gcc
CFLAGS = -Wall -m32
SRC = ./src
LIB = ./lib
INC = ./include
OBJ = ./obj

.PHONY : all
all: $(LIB)/libsched.a

$(LIB)/libsched.a: $(OBJ)/sched.o $(OBJ)/nlist2.o $(OBJ)/synch.o $(OBJ)/callout.o $(OBJ)/bio.o $(OBJ)/conf.o
	ar r $@ $?
	ranlib $@


$(OBJ)/callout.o : $(SRC)/callout.c $(INC)/callo.h $(INC)/sched.h $(INC)/var.h
	$(CC) $(CFLAGS) -I $(INC) -c $< -o $@


clean: 
	rm -f *~ $(SRC)/*~ $(INC)/*~ $(LIB)/libsched.a $(OBJ)/callout.o
