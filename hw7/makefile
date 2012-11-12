.SUFFIXES: .h .o .cpp

CC = main.cpp loc.cpp parser.cpp nano.cpp nano_guess.cpp
CC = g++
#CXXFLAGS = -O2 -std=c++0x
CXXFLAGS = -g
socklibs = -lnsl
LIBS = 
OBJS = main.o loc.o parser.o nano.o nano_guess.o
CCEXEC = nano

all:	$(CCEXEC) $(MOVEABLETEST) $(HUNTERBOUNCETEST)

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
	rm -f $(CCEXEC) $(MOVEABLETEST) $(HUNTERBOUNCETEST)
	rm -f core
