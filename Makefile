GCC_FLAGS = -Wall
all: application

peroraison.o	: peroraison.c peroraison.h
	gcc $(GCC_FLAGS) -c peroraison.c peroraison.h

InitConnexion.o	: InitConnexion.c peroraison.h
	gcc $(GCC_FLAGS) -c InitConnexion.c peroraison.h

textuel-pop.o	: textuel-pop.c peroraison.h
	gcc $(GCC_FLAGS) -c textuel-pop.c peroraison.h

main-pop.o	: main-pop.c peroraison.h
	gcc $(GCC_FLAGS) -c main-pop.c peroraison.h

application	: main-pop.o peroraison.o InitConnexion.o textuel-pop.o
	gcc $(GCC_FLAGS) -o application main-pop.o peroraison.o InitConnexion.o textuel-pop.o

clean	:
	rm -f *.o *.gch application