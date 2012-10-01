.SUFFIXES: .h .o .cpp

CC = main.cpp patient.cpp ambulance.cpp
CC = g++
CCOPTS = -g
LIBS = 
OBJS = main.o patient.o ambulance.o
CCEXEC = ambulance

all:	$(CCEXEC)

$(CCEXEC):	$(OBJS) makefile 
	@echo Linking $@ . . .
	$(CC) $(CCOPTS) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) -c $(CCOPTS) $<

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC)
	rm -f core