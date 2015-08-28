### makefile for simple programs
###########################################
INC=
LIB= -lpthread

CC=g++
CC_FLAG=-Wall

PRG=BTree_Demo
OBJ=*.o

$(PRG):$(OBJ)
	$(CC) $(OBJ) $(INC) $(LIB) -o $(PRG)
.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CC) $(INC) -c $*.cpp $(CC_FLAG)
.PRONY:clean
clean:
	rm -f $(OBJ) $(PRG)
