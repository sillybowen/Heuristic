.SUFFIXES: .h .o .cpp

CC = ClientSocket.cpp Socket.cpp moveable.cpp evasion.cpp hunter.cpp prey.cpp main.cpp
CC = g++
#CXXFLAGS = -O2 -std=c++0x
CXXFLAGS = -g
socklibs = -lnsl
LIBS = 
OBJS = ClientSocket.o Socket.o moveable.o evasion.o hunter.o prey.o main.o
CCEXEC = evasion

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