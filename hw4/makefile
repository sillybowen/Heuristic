.SUFFIXES: .h .o .cpp

CC = ClientSocket.cpp Socket.cpp dp_remove.cpp flat_board.cpp greedy_player.cpp main.cpp
CC = g++
#CXXFLAGS = -O2 -std=c++0x
CXXFLAGS = -g 
socklibs = -lnsl
LIBS = 
OBJS = ClientSocket.o Socket.o dp_remove.o flat_board.o greedy_player.o main.o
CCEXEC = notipping

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
