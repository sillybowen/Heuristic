.SUFFIXES: .h .o .cpp

CC = flat_board.cpp greedy_player.cpp main.cpp
CC = g++
#CXXFLAGS = -O2 -std=c++0x
CXXFLAGS = -g
LIBS = 
OBJS = flat_board.o greedy_player.o main.o
CCEXEC = notipping

all:	$(CCEXEC)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC)
	rm -f core
