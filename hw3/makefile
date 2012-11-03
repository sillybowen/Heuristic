.SUFFIXES: .h .o .cpp

CC = bowen_work_class.cpp patient.cpp ambulance.cpp aco_ambulance.cpp kmeans.cpp hospital_patient.cpp aco_graphs.cpp main.cpp
CC = g++
CXXFLAGS =  -std=c++0x -O2 
LIBS = 
OBJS = bowen_work_class.o patient.o ambulance.o aco_ambulance.o kmeans.o hospital_patient.o aco_graphs.o main.o
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
