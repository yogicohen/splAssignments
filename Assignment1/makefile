# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: fs

# Tool invocations
fs: bin/Files.o bin/Commands.o bin/Main.o bin/Environment.o bin/FileSystem.o
	@echo 'Building target: fs'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/fs bin/Files.o bin/Commands.o bin/Main.o bin/Environment.o bin/FileSystem.o $(LFLAGS)
	@echo 'Finished building target: fs'
	@echo ' '

# Depends on the source and header files
bin/Files.o: src/Files.cpp 
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Files.o src/Files.cpp
bin/Commands.o: src/Commands.cpp 
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Commands.o src/Commands.cpp
bin/Main.o: src/Main.cpp 
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp
bin/Environment.o: src/Environment.cpp 
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Environment.o src/Environment.cpp
bin/FileSystem.o: src/FileSystem.cpp 
	$(CC) $(CFLAGS) -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp


#Clean the build directory
clean: 
	rm -f bin/*
