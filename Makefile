CC =gcc -Wall -I$I
BIN=bin
INC=include
OBJ=obj
SRC=src
I=/usr/X11R6/include
L=/usr/X11R6/lib
l=X11

all: ${BIN}/application

$(OBJ)/peroraison.o	: $(SRC)/peroraison.c ${INC}/peroraison.h
	$(CC) -c -o $(OBJ)/peroraison.o $(SRC)/peroraison.c -I${INC}

$(OBJ)/InitConnexion.o	: $(SRC)/InitConnexion.c ${INC}/peroraison.h
	$(CC) -c -o $(OBJ)/InitConnexion.o $(SRC)/InitConnexion.c -I${INC}

$(OBJ)/textuel-pop.o	: $(SRC)/textuel-pop.c ${INC}/peroraison.h
	$(CC) -c -o $(OBJ)/textuel-pop.o $(SRC)/textuel-pop.c -I${INC}

$(OBJ)/clicable-pop.o	: $(SRC)/clicable-pop.c ${INC}/peroraison.h
	$(CC) -c -o $(OBJ)/clicable-pop.o $(SRC)/clicable-pop.c -I${INC}

$(OBJ)/main-pop.o	: $(SRC)/main-pop.c ${INC}/peroraison.h
	$(CC) -c -o $(OBJ)/main-pop.o $(SRC)/main-pop.c -I${INC}

$(BIN)/application	: $(OBJ)/main-pop.o $(OBJ)/peroraison.o $(OBJ)/InitConnexion.o $(OBJ)/textuel-pop.o $(OBJ)/clicable-pop.o
	$(CC) -L$L -l$l -o $(BIN)/application $(OBJ)/main-pop.o $(OBJ)/peroraison.o $(OBJ)/InitConnexion.o $(OBJ)/textuel-pop.o $(OBJ)/clicable-pop.o

clean	:
	rm -rf ${OBJ}/* ${BIN}/* ${INC}/*.gch
