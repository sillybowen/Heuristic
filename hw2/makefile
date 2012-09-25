DIRS :=  match match/MinCost match/GEOM

SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp)) matrix_graph.cpp traveling_salesmam.cpp mst.cpp matching.cpp NN.cpp evaluate.cpp nn_bowen.cpp trim.cpp genetic.cpp
OBJS := $(patsubst %.cpp, %.o, $(SOURCES))

CFLAGS := -O5 -D_NDEBUG -std=c++0x
CC := g++
LIBS := 
INCLUDES := 
LIBDIR := 

all: tsp_solver

eulerCircuitTest: matrix_graph.cpp euler_circuit_test.cpp
	g++ euler_circuit_test.cpp matrix_graph.cpp -std=c++0x -o eulerCircuitTest

tsp_solver: ${OBJS}
	${CC} ${CFLAGS} ${LIBDIR} -o $@ ${OBJS} ${LIBS}

.cpp.o:
	$(CC) $(CFLAGS) ${INCLUDES} $< -c -o $@

clean:
	rm -f ${OBJS} tsp_solver
