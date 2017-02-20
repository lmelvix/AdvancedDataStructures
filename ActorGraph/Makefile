# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
    CPPFLAGS += -O3
    LDFLAGS += -O3
else
    CPPFLAGS += -g
    LDFLAGS += -g
endif

all: pathfinder actorconnections extension

pathfinder: ActorGraph.o

actorconnections: ActorGraph.o UpTree.o util.o

extension: TwitterGraph.o

ActorGraph.o: ActorGraph.h

UpTree.o: UpTree.h

util.o: util.h

TwitterGraph.o: TwitterGraph.h

clean:
	rm -f pathfinder *.o core*

