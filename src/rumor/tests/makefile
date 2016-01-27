# GPP=g++
# CFLAGS=-std=c++11

# test1: test1.o
# 	$(GPP) -o $@ $< $(CFLAGS)

# test1.o: test1.cpp
# 	$(GPP) -c $< $(CFLAGS)

# \n para quebrar as linhas no foreach
# Cada linha de um makefile eh rodada num shell diferente.
# Se os comandos de cd estiverem na mesma linhas, tem que voltar na arvore
# de diretorios. Se estiverem em linhas diferentes, os caminhos podem ser
# referentes ao diretorio atual.
# $^ sao todos os feras de cima do lado direito
define \n


endef

GPP=g++
CFLAGS=-std=c++11 -g -O2 -lgmpxx -lgmp
LIBS=-L/usr/local/lib -lgsl -lgslcblas -lm
DIRS=../../stat/ ../../graph/ ../
DEPS=../../stat/stat.h ../../graph/graph.h ../rumor.h
OBJS=../../stat/stat.o ../../graph/graph.o ../rumor.o

all: makes

maketest1: makes test1

maketest2: makes test2

test1: test1.cpp $(OBJS)
	$(GPP) -o $@ $^ $(CFLAGS) $(LIBS)

test2: test2.cpp $(OBJS)
	$(GPP) -o $@ $^ $(CFLAGS) $(LIBS)

makes:
	$(foreach dir,$(DIRS), cd $(dir); make ${\n})


