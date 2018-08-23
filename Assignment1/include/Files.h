#ifndef FILES_H_
#define FILES_H_

using namespace std;
#include <string>
#include <vector>

class BaseFile {
private:
	string name;
	
public:
	BaseFile(string name);
	string getName() const;
	void setName(string newName);
	virtual int getSize() = 0;
	virtual bool isFile() = 0;
	virtual ~BaseFile()= default;
};

class File : public BaseFile {
private:
	int size;
		
public:
	File(string name, int size); // Constructor
	int getSize(); // Return the size of the file
	bool isFile();
	virtual ~File()= default;
};

class Directory : public BaseFile {
private:
	Directory *parent;
	vector<BaseFile*> children;

	void clean(); // for internal usage of rule of 5 methods
	void copy(const Directory &other); //for internal usage of rule of 5 methods
	void steal(Directory &other);//for internal usage of rule of 5 methods

public:
	Directory(string name, Directory *parent); // Constructor
	Directory *getParent() const; // Return a pointer to the parent of this directory
	void setParent(Directory *newParent); // Change the parent of this directory
	void addFile(BaseFile* file); // Add the file to children
	void removeFile(string name); // Remove the file with the specified name from children
	void removeFile(BaseFile* file); // Remove the file from children
	void sortByName(); // Sort children by name alphabetically (not recursively)
	void sortBySize(); // Sort children by size (not recursively)
	vector<BaseFile*> getChildren(); // Return children
	int getSize(); // Return the size of the directory (recursively)
	string getAbsolutePath();  //Return the path from the root to this

	bool isFile();


	//rule of 5
	virtual ~Directory();//destructor
	Directory(const Directory &other);//copy Constructor
	Directory& operator=(const Directory& other);//copy assignment operator
	Directory(Directory &&other);//move Constructor
	Directory& operator=(Directory &&other);//move assignment operator
};
	//assisting methods for sorting
	bool NameCompare(BaseFile *First,BaseFile *Second);
 	bool SizeCompare(BaseFile *First,BaseFile *Second);

#endif