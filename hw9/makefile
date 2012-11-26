.SUFFIXES: .h .o .cpp

CC = ClientSocket.cpp Socket.cpp game.cpp gamble.cpp main.cpp
CC = g++
CXXFLAGS = -O2
#CXXFLAGS = -g
socklibs = -lnsl
LIBPTHREAD = -lpthread
LIBS = 
OBJS = ClientSocket.o Socket.o game.o gamble.o main.o
CCEXEC = portfolio

all:	$(CCEXEC)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(LIBPTHREAD) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC)
	rm -f core
