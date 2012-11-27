.SUFFIXES: .h .o .cpp

CC = engine.cpp stock.cpp ClientSocket.cpp Socket.cpp game.cpp gamble.cpp main.cpp
CC = g++
CXXFLAGS = -O2
#CXXFLAGS = -g
socklibs = -lnsl
LIBPTHREAD = -lpthread
LIBS = 
OBJS = engine.o stock.o ClientSocket.o Socket.o game.o gamble.o main.o
CCEXEC = portfolio
ENGINETESTFILE = engineTest.cpp
ENGINETEST = engine_test
all:	$(CCEXEC) $(ENGINETEST)

$(CCEXEC):	$(OBJS) makefile
	@echo Linking $@ . . .
	$(CC) $(CXXFLAGS) $(socklibs) $(LIBPTHREAD) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) $(CXXFLAGS) $<
$(ENGINETEST): $(ENGINETESTFILE) engine.o stock.o
	$(CC) $(ENGINETESTFILE) engine.o stock.o -o $(ENGINETEST)
run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC) $(ENGINETEST)
	rm -f core
