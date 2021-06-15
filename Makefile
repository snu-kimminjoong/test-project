CC = g++
CFLAGS = -c -g
OBJECTS = turnbasedGame.o

program : $(OBJECTS)
	$(CC) -o turnbasedGame turnbasedGame.o

turnbasedGame.o : turnbasedGame.cpp
	$(CC) $(CFLAGS) turnbasedGame.cpp