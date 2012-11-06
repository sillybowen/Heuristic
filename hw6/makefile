.SUFFIXES: .h .o .cpp

CC = ClientSocket.cpp Socket.cpp moveable.cpp evasion.cpp hunter.cpp prey.cpp bowen_hunter.cpp bowen_prey.cpp jinil_hunter.cpp jinil_prey.cpp random_prey.cpp main.cpp
CC = g++
#CXXFLAGS = -O2 -std=c++0x
CXXFLAGS = -g
socklibs = -lnsl
LIBS = ./jsoncpp-src-0.5.0/libs/linux-gcc-4.5.2/libjson_linux-gcc-4.5.2_libmt.so
OBJS = ClientSocket.o Socket.o moveable.o evasion.o hunter.o prey.o bowen_hunter.o bowen_prey.o jinil_hunter.o jinil_prey.o random_prey.o main.o
CCEXEC = evasion
MOVEABLETESTFILE = adj_wall_test.cpp
MOVEABLETEST = adj_wall_test
HUNTERBOUNCETEST = hunt_bounce_test
HUNTERBOUNCETESTFILE = hunter_bounce_test.cpp

all:	$(CCEXEC) $(MOVEABLETEST) $(HUNTERBOUNCETEST)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<

$(MOVEABLETEST): $(MOVEABLETESTFILE) moveable.o
	$(CC) $(MOVEABLETESTFILE) moveable.o -lgtest -lpthread -o $(MOVEABLETEST)

$(HUNTERBOUNCETEST) : $(HUNTERBOUNCETESTFILE) moveable.o
	$(CC) $(HUNTERBOUNCETESTFILE) moveable.o -lgtest -lpthread -o $(HUNTERBOUNCETEST)

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC) $(MOVEABLETEST) $(HUNTERBOUNCETEST)
	rm -f core
