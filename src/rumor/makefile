GPP=g++
CFLAGS=-std=c++11 -g -O2 -lgmpxx -lgmp
LIBS=-L/usr/local/lib -lgsl -lgslcblas -lm
DEPS=../graph/graph.h ../stat/stat.h
OBJ=../graph/graph.o ../stat/stat.o

%.o: %.c $(DEPS)
	$(GPP) -c -o $@ $< $(CFLAGS)

rumor: rumor.o $(OBJ)
	$(GPP) -o $@ $^ $(CFLAGS) $(LIBS)

