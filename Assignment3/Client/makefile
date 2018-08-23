CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system -lboost_thread

all: BBclient
	g++ -o bin/BBclient bin/connectionHandler.o bin/Task1.o bin/Task2.o bin/BBclient.o $(LDFLAGS) 

BBclient: bin/connectionHandler.o bin/Task1.o bin/Task2.o bin/BBclient.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Task1.o: src/Task1.cpp
	g++ $(CFLAGS) -o bin/Task1.o src/Task1.cpp

bin/Task2.o: src/Task2.cpp
	g++ $(CFLAGS) -o bin/Task2.o src/Task2.cpp

bin/BBclient.o: src/BBclient.cpp
	g++ $(CFLAGS) -o bin/BBclient.o src/BBclient.cpp
	
.PHONY: clean
clean: 
	rm -f bin/*
