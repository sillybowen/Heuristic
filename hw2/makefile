DIRS :=  match match/MinCost match/GEOM

SOURCES := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp)) matrix_graph.cpp traveling_salesmam.cpp mst.cpp matching.cpp
OBJS := $(patsubst %.cpp, %.o, $(SOURCES))

CFLAGS := -O5 -D_NDEBUG
CC := g++
LIBS := 
INCLUDES := 
LIBDIR := 

all: travel

travel: ${OBJS}
	${CC} ${CFLAGS} ${LIBDIR} -o $@ ${OBJS} ${LIBS}

.cpp.o:
	$(CC) $(CFLAGS) ${INCLUDES} $< -c -o $@

clean:
	rm -f ${OBJS} travel
