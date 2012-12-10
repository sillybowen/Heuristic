.SUFFIXES: .h .o .cpp

CC = main.cpp dp_remove.cpp
CC = g++
CXXFLAGS =  -O2
LIBS = 
OBJS =  main.o dp_remove.o
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
