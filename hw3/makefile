.SUFFIXES: .h .o .cpp

CC = patient.cpp ambulance.cpp kmeans.cpp hospital_patient.cpp aco_graphs.cpp main.cpp
CC = g++
CXXFLAGS = -g -std=c++0x
LIBS = 
OBJS = patient.o ambulance.o kmeans.o hospital_patient.o aco_graphs.o main.o
CCEXEC = ambulance

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
