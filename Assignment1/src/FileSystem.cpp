#include "../include/FileSystem.h"
#include "../include/Commands.h"
#include "../include/GlobalVariables.h"

#include <iostream>
#include <algorithm>
using namespace std;


FileSystem::FileSystem():rootDirectory(new Directory("/", nullptr)),workingDirectory(rootDirectory){

}

Directory& FileSystem::getRootDirectory() const{
    return *rootDirectory;
}

Directory& FileSystem::getWorkingDirectory() const{
    return  *workingDirectory;
}

void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory){
    workingDirectory=newWorkingDirectory;
}

FileSystem::~FileSystem(){
    if((verbose==1) | (verbose==3)){
        cout<<"FileSystem::~FileSystem()"<<endl;
    }
    clean();
}

void FileSystem::clean(){
    delete(rootDirectory);
    rootDirectory= nullptr;
    workingDirectory= nullptr;
}

FileSystem::FileSystem(const FileSystem &other):rootDirectory(new Directory("/", nullptr)),workingDirectory(rootDirectory){
    if((verbose==1) | (verbose==3)){
        cout<<"FileSystem::FileSystem(const FileSystem &other)"<<endl;
    }
    CdCommand cd(other.workingDirectory->getAbsolutePath());
    copy(other);
    cd.execute(*this);
}

void FileSystem::copy(const FileSystem &other) {
    for(size_t i(0);i<other.getRootDirectory().getChildren().size();i++)
        if(other.getRootDirectory().getChildren()[i]->isFile())
            rootDirectory->getChildren().push_back(new File (other.getRootDirectory().getChildren()[i]->getName(),other.getRootDirectory().getChildren()[i]->getSize()));
        else {
            Directory &B = (dynamic_cast<Directory &> (*other.getRootDirectory().getChildren()[i]));
            rootDirectory->getChildren().push_back(new Directory(B));
        }
}

FileSystem& FileSystem:: operator=(const FileSystem& other){
    if((verbose==1) | (verbose==3)){
        cout<<"FileSystem& FileSystem:: operator=(const FileSystem& other)"<<endl;
    }
    if(this!=&other) {
        clean();
        copy (other);
    }
    return *this;
}

FileSystem::FileSystem(FileSystem &&other):rootDirectory(new Directory("/", nullptr)),workingDirectory(rootDirectory){
    if((verbose==1) | (verbose==3)){
        cout<<"FileSystem::FileSystem(FileSystem &&other)"<<endl;
    }
    steal(other);
}

void FileSystem::steal(FileSystem &other){
    rootDirectory=other.rootDirectory;
    setWorkingDirectory(other.workingDirectory);

    other.rootDirectory=nullptr;
    other.workingDirectory=other.rootDirectory;
}

FileSystem& FileSystem::operator=(FileSystem &&other){
    if((verbose==1) | (verbose==3)){
        cout<<"FileSystem& FileSystem::operator=(FileSystem &&other)"<<endl;
    }
    clean();
    steal(other);
    return *this;
}