.SUFFIXES: .h .o .cpp

CC = ClientSocket.cpp Socket.cpp game.cpp server_game.cpp person.cpp \
	 matchmaker.cpp gradient_matcher.cpp main.cpp
CC = g++
#CXXFLAGS = -O2
CXXFLAGS = -g
socklibs = -lnsl
LIBS = 
OBJS = ClientSocket.o Socket.o game.o server_game.o person.o matchmaker.o\
	   gradient_matcher.o main.o
CCEXEC = dating
RANDOMPERSONTEST = random_person_test
RANDOMPERSONTESTFILE = random_person_test.cpp

all:	$(CCEXEC) $(RANDOMPERSONTEST)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<

$(RANDOMPERSONTEST) : $(RANDOMPERSONTESTFILE) person.o
	$(CC) $(RANDOMPERSONTESTFILE) person.o -lgtest -lpthread -o $(RANDOMPERSONTEST)

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC) $(RANDOMPERSONTEST)
	rm -f core
