.SUFFIXES: .h .o .cpp

CC = ClientSocket.cpp Socket.cpp game.cpp server_game.cpp person.cpp \
	 matchmaker.cpp gradient_matcher.cpp thread_pool_normal.cpp thread.cpp \
	 thread_registry.cpp log_message.cpp log_writer.cpp main.cpp
CC = g++
#CXXFLAGS = -O2
CXXFLAGS = -g
socklibs = -lnsl
LIBPTHREAD = -lpthread
LIBS = 
OBJS = ClientSocket.o Socket.o game.o server_game.o person.o matchmaker.o\
	   gradient_matcher.o thread_pool_normal.o thread.o thread_registry.o \
	   log_message.o log_writer.o main.o
CCEXEC = dating
RANDOMPERSONTEST = random_person_test
RANDOMPERSONTESTFILE = random_person_test.cpp

all:	$(CCEXEC) $(RANDOMPERSONTEST)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(LIBPTHREAD) $(OBJS) -o $@ $(LIBS)

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
