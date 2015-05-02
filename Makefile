CC =gcc -Wall
BIN=bin
INC=include
OBJ=obj
SRC=src

all: ${BIN}/application

$(OBJ)/peroraison.o	: $(SRC)/peroraison.c ${INC}/peroraison.h
	gcc $(GCC_FLAGS) -c -o $(OBJ)/peroraison.o $(SRC)/peroraison.c -I${INC}

$(OBJ)/InitConnexion.o	: $(SRC)/InitConnexion.c ${INC}/peroraison.h
	gcc $(GCC_FLAGS) -c -o $(OBJ)/InitConnexion.o $(SRC)/InitConnexion.c -I${INC}

$(OBJ)/textuel-pop.o	: $(SRC)/textuel-pop.c ${INC}/peroraison.h
	gcc $(GCC_FLAGS) -c -o $(OBJ)/textuel-pop.o $(SRC)/textuel-pop.c -I${INC}

$(OBJ)/main-pop.o	: $(SRC)/main-pop.c ${INC}/peroraison.h
	gcc $(GCC_FLAGS) -c -o $(OBJ)/main-pop.o $(SRC)/main-pop.c -I${INC}

$(BIN)/application	: $(OBJ)/main-pop.o $(OBJ)/peroraison.o $(OBJ)/InitConnexion.o $(OBJ)/textuel-pop.o
	gcc $(GCC_FLAGS) -o $(BIN)/application $(OBJ)/main-pop.o $(OBJ)/peroraison.o $(OBJ)/InitConnexion.o $(OBJ)/textuel-pop.o

clean	:
	rm -f ${OBJ}/* ${BIN}/* ${INC}/*.gch
