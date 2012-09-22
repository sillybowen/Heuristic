.SUFFIXES: .h .o .cpp

CC = matrix_graph.cpp traveling_salesmam.cpp mst.cpp matching.cpp
CC = g++
CCOPTS = -g
LIBS = 
OBJS = mst.o matrix_graph.o traveling_salesmam.o matching.o
CCEXEC = travel

all:	$(CCEXEC)

$(CCEXEC):	$(OBJS) makefile 
	@echo Linking $@ . . .
	$(CC) $(CCOPTS) $(OBJS) -o $@ $(LIBS)

%.o:	%.c
	@echo Compiling $< . . .
	$(CC) -c -O2$(CCOPTS) $<

run:	all
	./$(CCEXEC)

clean:
	rm -f $(OBJS)
	rm -f $(CCEXEC)
	rm -f core
