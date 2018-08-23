#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;

    void clean(); // for internal usage of rule of 5 methods
    void copy(const Environment &other); //for internal usage of rule of 5 methods
    void steal(Environment &other);//for internal usage of rule of 5 methods

public:
	Environment();
	void start();
	FileSystem& getFileSystem(); // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands

    //rule of 5 functions:
    virtual ~Environment();
    Environment(const Environment &other);//copy Constructor
    Environment& operator=(const Environment& other);//copy assignment operator
    Environment(Environment &&other);//move Constructor
    Environment& operator=(Environment &&other);//move assignment operator
};

#endif