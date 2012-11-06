.SUFFIXES: .h .o .cpp

CC = ClientSocket.cpp Socket.cpp moveable.cpp evasion.cpp hunter.cpp prey.cpp bowen_hunter.cpp bowen_prey.cpp jinil_hunter.cpp jinil_prey.cpp random_prey.cpp main.cpp
CC = g++
#CXXFLAGS = -O2 -std=c++0x
CXXFLAGS = -g
socklibs = -lnsl
LIBS = ./jsoncpp-src-0.5.0/libs/linux-gcc-4.6/libjson_linux-gcc-4.6_libmt.so
OBJS = ClientSocket.o Socket.o moveable.o evasion.o hunter.o prey.o bowen_hunter.o bowen_prey.o jinil_hunter.o jinil_prey.o random_prey.o main.o
CCEXEC = evasion
MOVEABLETESTFILE = adj_wall_test.cpp
MOVEABLETEST = moveable_test

all:	$(CCEXEC) $(MOVEABLETEST)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<

$(MOVEABLETEST): $(MOVEABLETESTFILE)
	$(CC) $(MOVEABLETESTFILE) moveable.o -lgtest -lpthread -o $(MOVEABLETEST)

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC) $(MOVEABLETEST)
	rm -f core
