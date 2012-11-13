.SUFFIXES: .h .o .cpp

CC = ClientSocket.cpp Socket.cpp main.cpp loc.cpp parser.cpp nano.cpp nano_guess.cpp work.cpp game.cpp
CC = g++
#CXXFLAGS = -O2 -std=c++0x
CXXFLAGS = -g
socklibs = -lnsl
LIBS = 
OBJS = ClientSocket.o Socket.o main.o loc.o parser.o nano.o nano_guess.o work.o game.o
CCEXEC = nano
WORKTESTFILE = work_test.cpp
WORKTEST = work_test
all:	$(CCEXEC) $(WORKTEST)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<

$(WORKTEST): $(WORKTESTFILE) parser.o nano.o work.o loc.o
	$(CC) $(WORKTESTFILE) nano_guess.o parser.o nano.o work.o loc.o -o $(WORKTEST)

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC) $(WORKTEST) 
	rm -f core
