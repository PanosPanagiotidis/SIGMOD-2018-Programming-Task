CC=g++
HDIR=-I'./header/'
CFLAGS = -g -o3 -Wall -pedantic -Wunreachable-code

JOIN: ./src/testmain.o ./src/helper_functions.o ./src/results.o ./src/daindex.o ./src/parsing.o ./src/relationops.o ./src/relation_l.o
	$(CC) $(CFLAGS)	-o $@.out ./src/testmain.o ./src/helper_functions.o ./src/results.o ./src/daindex.o ./src/parsing.o ./src/relationops.o ./src/relation_l.o

testmain.o:	./src/testmain.cpp
	$(CC) $(CFLAGS) -c ./src/testmain.cpp

helper_functions.o:	./src/helper_functions.c
	$(CC) $(CFLAGS) -c ./src/helper_functions.c

results.o:	./src/results.cpp ./header/daindex.h
	$(CC) $(CFLAGS) -c ./src/results.cpp

daindex.o: 	./src/daindex.c
	$(CC) $(CFLAGS) -c ./src/daindex.c

parsing.o:	./src/parsing.cpp
	$(CC) $(CFLAGS) -c ./src/parsing.cpp

relation_l.o:	./src/relation_l.cpp
	$(CC) $(CFLAGS) -c ./src/relation_l.cpp

relationops.o: ./src/relationops.cpp
	$(CC) $(CFLAGS) -c ./src/relationops.cpp

clean:
	$(RM) *.out
	$(RM) ./header/*.h.gch
