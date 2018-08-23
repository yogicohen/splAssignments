#include "../include/Files.h"
#include "../include/GlobalVariables.h"
#include <iostream>
#include <algorithm>

/********BaseFile implementation************/
BaseFile::BaseFile(string name):name(name) {}

string BaseFile:: getName() const{
    return name;
}

void BaseFile::setName(string newName){
    name=newName;
}


/********File implementation************/

File::File(string name, int size): BaseFile(name),size(size){}

int File::getSize(){
    return size;
}

bool File::isFile() {return true;}



/********Directory implementation************/

Directory::Directory(string name, Directory *parent):BaseFile(name),parent(parent),children(){}

Directory* Directory::getParent() const{
    if(parent!=nullptr)
        return parent;
    return nullptr;
}

void Directory::setParent(Directory *newParent){
    if(parent==nullptr){
        parent=newParent;
        (*newParent).children.push_back(this);
    }
    else{
        bool found(false);
        for (size_t i(0);i<(*parent).children.size() && (!found);i++){
            if((*parent).children[i]->getName()==getName()) {
                (*parent).children.erase((*parent).children.begin() + i);
                found=true;
            }
        }
        parent=newParent;
        (*newParent).children.push_back(this);
    }

}

void Directory::addFile(BaseFile* file){
    for(size_t i(0);i<children.size();i++)
        if ((*children[i]).getName()==(*file).getName()) {
            cout<<"The directory already exists"<<endl;
            return;
        }
    children.push_back(file);
}

void Directory::removeFile(string name){
    bool found(false);
    for(size_t i(0);i<children.size() && (!found);i++){
        if ((*children[i]).getName()==name){
            found=true;
            delete(children[i]);
            children.erase(children.begin()+i);
        }
    }
}

void Directory::removeFile(BaseFile *file) {
    bool found(false);
    for(size_t i(0);i<children.size() && (!found);i++){
        if ((*children[i]).getName()==(*file).getName()){
            found=true;
            delete(children[i]);
            children.erase(children.begin()+i);
        }
    }
}
//using sort algorithm
void Directory::sortByName(){
    sort(children.begin(),children.end(),NameCompare);
}

//using sort algorithm
void Directory::sortBySize(){
    sort(children.begin(),children.end(),SizeCompare);

    for (size_t i = 0; i <children.size()-1 ; ++i) {

        if(children[i]->getSize()==children[i+1]->getSize()){
            size_t from(i);
            size_t to(i+1);
            for (size_t j = to+1; j < children.size() ; ++j) {
                if (children[i]->getSize()==children[j]->getSize())
                    to++;
            }
            sort((children.begin()+from),(children.begin()+to),NameCompare);
            i=to;
        }
    }
}

//boolean assisting method for name comparing
bool NameCompare(BaseFile *First,BaseFile *Second){
    return (First->getName().compare(Second->getName())<0);
}

//boolean assisting method for size comparing
bool SizeCompare(BaseFile *First,BaseFile *Second){
    return (First->getSize() < Second->getSize());
}

vector<BaseFile*> Directory::getChildren(){
    return children;
}

int Directory::getSize(){
    int sum(0);
    for(size_t i(0);i<children.size();i++)
        sum += children[i]->getSize();

    return sum;
}

string Directory::getAbsolutePath(){
    if (parent == nullptr)
        return "/";
    string s="/"+getName();

    return (parent->getAbsolutePath())+s;
}

bool Directory::isFile() {return false;}
/********Directory rule of five implementation************/

Directory::~Directory() {
    if((verbose==1) | (verbose==3)){
        cout<<"Directory::~Directory()"<<endl;
    }
    clean();
}

void Directory::clean() {
    parent= nullptr;

    while (children.size() != 0)//in case the directory still contains files
        removeFile(children[0]);

    children.clear();
    return;
}

Directory::Directory(const Directory &other):BaseFile(other.getName()),parent(other.parent),children(){
    if((verbose==1) | (verbose==3)){
        cout<<"Directory::Directory(const Directory &other)"<<endl;
    }

    copy(other);
}

void Directory::copy(const Directory &other) {
    for (size_t i(0);i<other.children.size();i++)
    {
        if (other.children[i]->isFile())
            children.push_back(new File(other.children[i]->getName(),other.children[i]->getSize()));
        else{
            Directory& A = (dynamic_cast<Directory &>(*other.children[i]));
            children.push_back(new Directory(A));
        }
    }
}

Directory& Directory::operator=(const Directory& other){
    if((verbose==1) | (verbose==3)){
        cout<<"Directory& Directory::operator=(const Directory& other)"<<endl;
    }
    if (this!=&other) {
        clean();
        copy(other);
    }
    return *this;
}

Directory::Directory(Directory &&other):BaseFile(other.getName()),parent(nullptr),children(){
    if((verbose==1) | (verbose==3)){
        cout<<"Directory::Directory(Directory &&other)"<<endl;
    }
    steal(other);
}

void Directory::steal(Directory &other){
    parent=other.parent;
    children=other.getChildren();

    other.parent=nullptr;
    other.children.clear();
}

Directory& Directory::operator=(Directory &&other){
    if((verbose==1) | (verbose==3)){
        cout<<"Directory& Directory::operator=(Directory &&other)"<<endl;
    }
    clean();
    steal(other);
    return *this;
}

