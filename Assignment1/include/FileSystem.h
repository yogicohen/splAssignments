#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"


class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;

	void clean(); // for internal usage of rule of 5 methods
	void copy(const FileSystem &other); //for internal usage of rule of 5 methods
	void steal(FileSystem &other);//for internal usage of rule of 5 methods

public:
	FileSystem();
	Directory& getRootDirectory() const; // Return reference to the root directory
	Directory& getWorkingDirectory() const; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system


	//rule of 5
	virtual ~FileSystem();//destructor
	FileSystem(const FileSystem &other);//copy Co nstructor
	FileSystem& operator=(const FileSystem& other);//copy assignment operator
	FileSystem(FileSystem &&other);//move Constructor
	FileSystem& operator=(FileSystem &&other);//move assignment operator
};


#endif
