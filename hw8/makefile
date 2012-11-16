.SUFFIXES: .h .o .cpp

CC = ClientSocket.cpp Socket.cpp game.cpp server_game.cpp person.cpp main.cpp
CC = g++
#CXXFLAGS = -O2 -std=c++0x
CXXFLAGS = -g
socklibs = -lnsl
LIBS = 
OBJS = ClientSocket.o Socket.o game.o server_game.o person.o main.o
CCEXEC = dating

all:	$(CCEXEC)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC)
	rm -f core
