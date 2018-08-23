#include "../include/Commands.h"
#include "../include/GlobalVariables.h"
#include <iostream>
using namespace std;

BaseCommand::BaseCommand(string args):args(args) {}

string BaseCommand::getArgs() {
    return args;
}

BaseCommand::~BaseCommand() {}

PwdCommand::PwdCommand(string args):BaseCommand(args) {}

void PwdCommand::execute(FileSystem &fs) {
    if((verbose==2) | (verbose==3)){
        cout<<this->toString()<<endl;
    }
    string print=fs.getWorkingDirectory().getAbsolutePath();
    if((print.size()>1)&&(print[0]=='/')&(print[1]=='/'))
        print=print.substr(1);
    cout<<print<<endl;
}

string PwdCommand::toString() {
    return "pwd";
}
PwdCommand::~PwdCommand() {}

CdCommand::CdCommand(string args):BaseCommand(args) {}


void CdCommand::execute(FileSystem &fs) {
    if((verbose==2) | (verbose==3)){
        cout<<this->toString()+' '+this->getArgs()<<endl;
    }

    Directory* toReturn = &(fs.getWorkingDirectory());
    string myArg = getArgs();
    string currentName = "";

    if (myArg.size() == 0) {
        cout << "The system cannot find the path specified" << endl;
        return;
    }

    if(myArg.size()!=0)
        myArg=myArg.substr(myArg.find_first_not_of(' '));

    //the first char is '/' means we need to go to root directory
    if (myArg[0] == '/') {
        fs.setWorkingDirectory(&fs.getRootDirectory());
        myArg = myArg.substr(1);
    }


    while (myArg.size() != 0) {
        if (!(isLegalChar(myArg[0]))) {
            cout << "The system cannot find the path specified" << endl;
            fs.setWorkingDirectory(toReturn);
            return;
        } else {
            if (myArg[0] == '/') {
                Directory &currDir = fs.getWorkingDirectory();
                if ((currDir.getChildren().size() == 0) & (currentName.size() != 0)) {
                    cout << "The system cannot find the path specified" << endl;
                    fs.setWorkingDirectory(toReturn);
                    return;
                } else {
                    bool found(false);
                    for (size_t i(0); (i < currDir.getChildren().size()) & (!found); i++) {
                        if ((!currDir.getChildren()[i]->isFile()) && (currDir.getChildren()[i]->getName() == currentName)) {
                            fs.setWorkingDirectory((Directory*)(currDir.getChildren()[i]));
                            myArg = myArg.substr(1);
                            currentName = "";
                            found = true;
                        }
                    }
                    if (!found) {
                        cout << "The system cannot find the path specified" << endl;
                        fs.setWorkingDirectory(toReturn);
                        return;
                    }
                }
            }

            if (myArg[0] == '.') {
                if ((myArg.size()>2) && (myArg[1] == '.') & (myArg[2] == '/')) {
                    if (&fs.getWorkingDirectory() == &fs.getRootDirectory()) {
                        cout << "The system cannot find the path specified" << endl;
                        fs.setWorkingDirectory(toReturn);
                        return;
                    } else {
                        fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
                        myArg = myArg.substr(3);
                    }
                }
                else{
                    if((myArg.size()>=2)& (myArg[1]=='.')){
                        if (&fs.getWorkingDirectory() == &fs.getRootDirectory()) {
                            cout << "The system cannot find the path specified" << endl;
                            fs.setWorkingDirectory(toReturn);
                            return;
                        } else {
                            fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
                            myArg = myArg.substr(2);
                        }
                    }
                }
            } else {
                bool toContinue(true);
                while (isLegalChar(myArg[0]) & toContinue) {
                    if ((myArg[0] == '.') | (myArg[0] == '/'))
                        toContinue = false;
                    else {
                        currentName = currentName + myArg[0];
                        myArg = myArg.substr(1);
                    }
                }
            }
        }
    }//while

    //the myArg string is empty and there was no '/' at the end
    if ((myArg.size() == 0) & (currentName.size() != 0)) {
        Directory &currDir = fs.getWorkingDirectory();
        if (currDir.getChildren().size() == 0) {
            cout << "The system cannot find the path specified" << endl;
            fs.setWorkingDirectory(toReturn);
            return;
        } else {
            bool found(false);
            for (size_t i(0); (i < currDir.getChildren().size()) & (!found); i++) {
                if ((!currDir.getChildren()[i]->isFile()) && (currDir.getChildren()[i]->getName() == currentName)) {
                    fs.setWorkingDirectory((Directory*)(currDir.getChildren()[i]));
                    found = true;
                }
            }
            if (!found) {
                cout << "The system cannot find the path specified" << endl;
                fs.setWorkingDirectory(toReturn);
                return;
            }
        }
    }
}

bool isLegalChar(char a){
    if(((a>='a') & (a<='z'))|((a>='A') & (a<='Z'))|((a>='0') & (a<='9'))|(a=='.')|(a=='/')|(a==' '))
        return true;
    else return false;
}

string CdCommand::toString() {
    return "cd";
}

CdCommand::~CdCommand() {}

bool CdCommand::executeBool(FileSystem &fs) {
    string myArg = getArgs();
    string currentName = "";

    if (myArg.size() == 0) {
        return false;
    }
    Directory* toReturn = &fs.getWorkingDirectory();

    if(myArg.size()!=0)
        myArg=myArg.substr(myArg.find_first_not_of(' '));

    //the first char is '/' means we need to go to root directory
    if (myArg[0] == '/') {
        fs.setWorkingDirectory(&fs.getRootDirectory());
        myArg = myArg.substr(1);
    }


    while (myArg.size() != 0) {
        if (!(isLegalChar(myArg[0]))) {
            fs.setWorkingDirectory(toReturn);
            return false;
        } else {
            if (myArg[0] == '/') {
                Directory &currDir = fs.getWorkingDirectory();
                if ((currDir.getChildren().size() == 0) & (currentName.size() != 0)) {
                    fs.setWorkingDirectory(toReturn);
                    return false;
                } else {
                    bool found(false);
                    for (size_t i(0); (i < currDir.getChildren().size()) & (!found); i++) {
                        if ((!currDir.getChildren()[i]->isFile()) && (currDir.getChildren()[i]->getName() == currentName)) {
                            fs.setWorkingDirectory((Directory*)(currDir.getChildren()[i]));
                            myArg = myArg.substr(1);
                            currentName = "";
                            found = true;
                        }
                    }
                    if (!found) {
                        fs.setWorkingDirectory(toReturn);
                        return false;
                    }
                }
            }

            if (myArg[0] == '.') {
                if ((myArg.size()>2)&&(myArg[1] == '.') & (myArg[2] == '/')) {
                    if (&fs.getWorkingDirectory() == &fs.getRootDirectory()) {
                        fs.setWorkingDirectory(toReturn);
                        return false;
                    } else {
                        fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
                        myArg = myArg.substr(3);
                    }
                }
                else{
                    if ((myArg.size()>=2)&&(myArg[1] == '.')) {
                        if (&fs.getWorkingDirectory() == &fs.getRootDirectory()) {
                            fs.setWorkingDirectory(toReturn);
                            return false;
                        } else {
                            fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
                            myArg = myArg.substr(2);
                        }
                    }
                }
            } else {
                bool toContinue(true);
                while ((isLegalChar(myArg[0])) & (toContinue)) {
                    if ((myArg[0] == '.') | (myArg[0] == '/'))
                        toContinue = false;
                    else {
                        currentName = currentName + myArg[0];
                        myArg = myArg.substr(1);
                    }
                }
            }
        }
    }//while

    //the myArg string is empty and there was no '/' at the end
    if ((myArg.size() == 0) & (currentName.size() != 0)) {
        Directory &currDir = fs.getWorkingDirectory();
        if (currDir.getChildren().size() == 0) {
            fs.setWorkingDirectory(toReturn);
            return false;
        } else {
            bool found(false);
            for (size_t i(0); (i < currDir.getChildren().size()) & (!found); i++) {
                if ((!currDir.getChildren()[i]->isFile()) && (currDir.getChildren()[i]->getName() == currentName)) {
                    fs.setWorkingDirectory((Directory*)(currDir.getChildren()[i]));
                    found = true;
                }
            }
            if (!found) {
                fs.setWorkingDirectory(toReturn);
                return false;
            }
        }
    }
    fs.setWorkingDirectory(toReturn);
    return true;
}

bool CdCommand::executeBoolLS(FileSystem &fs) {
    string myArg = getArgs();
    string currentName = "";

    if (myArg.size() == 0) {
        return false;
    }

    if(myArg.size()!=0)
        myArg=myArg.substr(myArg.find_first_not_of(' '));

    //the first char is '/' means we need to go to root directory
    if (myArg[0] == '/') {
        fs.setWorkingDirectory(&fs.getRootDirectory());
        myArg = myArg.substr(1);
    }


    while (myArg.size() != 0) {
        if (!(isLegalChar(myArg[0]))) {
            return false;
        } else {
            if (myArg[0] == '/') {
                Directory &currDir = fs.getWorkingDirectory();
                if ((currDir.getChildren().size() == 0) & (currentName.size() != 0)) {
                    return false;
                } else {
                    bool found(false);
                    for (size_t i(0); (i < currDir.getChildren().size()) & (!found); i++) {
                        if ((!currDir.getChildren()[i]->isFile()) && (currDir.getChildren()[i]->getName() == currentName)) {
                            fs.setWorkingDirectory((Directory*)(currDir.getChildren()[i]));
                            myArg = myArg.substr(1);
                            currentName = "";
                            found = true;
                        }
                    }
                    if (!found) {
                        return false;
                    }
                }
            }

            if (myArg[0] == '.') {
                if ((myArg.size()>2)&&(myArg[1] == '.') & (myArg[2] == '/')) {
                    if (&fs.getWorkingDirectory() == &fs.getRootDirectory()) {
                        return false;
                    } else {
                        fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
                        myArg = myArg.substr(3);
                    }
                }
                else{
                    if ((myArg.size()>=2)&&(myArg[1] == '.')) {
                        if (&fs.getWorkingDirectory() == &fs.getRootDirectory()) {
                            return false;
                        } else {
                            fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
                            myArg = myArg.substr(2);
                        }
                    }
                }
            } else {
                bool toContinue(true);
                while ((isLegalChar(myArg[0])) & (toContinue)) {
                    if ((myArg[0] == '.') | (myArg[0] == '/'))
                        toContinue = false;
                    else {
                        currentName = currentName + myArg[0];
                        myArg = myArg.substr(1);
                    }
                }
            }
        }
    }//while

    //the myArg string is empty and there was no '/' at the end
    if ((myArg.size() == 0) & (currentName.size() != 0)) {
        Directory &currDir = fs.getWorkingDirectory();
        if (currDir.getChildren().size() == 0) {
            return false;
        } else {
            bool found(false);
            for (size_t i(0); (i < currDir.getChildren().size()) & (!found); i++) {
                if ((!currDir.getChildren()[i]->isFile()) && (currDir.getChildren()[i]->getName() == currentName)) {
                    fs.setWorkingDirectory((Directory*)(currDir.getChildren()[i]));
                    found = true;
                }
            }
            if (!found) {
                return false;
            }
        }
    }
    return true;
}



LsCommand::LsCommand(string args):BaseCommand(args) {}

void LsCommand::execute(FileSystem &fs) {
    if((verbose==2) | (verbose==3)){
        if(this->getArgs().size()>0)
            cout<<this->toString()+' '+this->getArgs()<<endl;
        else
            cout<<this->toString()<<endl;
    }

    string myArg=getArgs();
    Directory* toReturn = &(fs.getWorkingDirectory());

    if (myArg.size()==0){
        Directory& currDir = fs.getWorkingDirectory();
        string toPrint;
        currDir.sortByName();
        for(size_t i(0);i<currDir.getChildren().size();i++){
            toPrint="";

            if(currDir.getChildren()[i]->isFile())
                toPrint +="FILE\t";
            else toPrint +="DIR\t";

            toPrint += currDir.getChildren()[i]->getName()+"\t";
            /*toPrint += currDir.getChildren()[i]->getSize();*/

            cout<<toPrint;
            cout<<(currDir.getChildren()[i]->getSize())<<endl;
        }
    }

    else if((myArg[0]!='[') & (myArg[0]!='-')) {//<path>
        Directory &currentDirToReSet = fs.getWorkingDirectory();
        CdCommand cd(myArg);
        bool didCd=cd.executeBoolLS(fs);
        if(didCd) {
            Directory &currDir = fs.getWorkingDirectory();
            string toPrint;
            currDir.sortByName();
            for (size_t i(0); i < currDir.getChildren().size(); i++) {
                toPrint = "";

                if (currDir.getChildren()[i]->isFile())
                    toPrint += "FILE\t";
                else toPrint += "DIR\t";

                toPrint += currDir.getChildren()[i]->getName() + "\t";
                /*toPrint += currDir.getChildren()[i]->getSize();*/

                cout << toPrint;
                cout<<(currDir.getChildren()[i]->getSize())<<endl;
            }
            fs.setWorkingDirectory(&currentDirToReSet);
        }
        else{
            cout << "The system cannot find the path specified" << endl;
            fs.setWorkingDirectory(toReturn);
            return;
        }
    }

    else if(((myArg.size()>=4) && (myArg.substr(0,4)=="[-s]")) | ((myArg.size()>=2) && (myArg.substr(0,2)=="-s"))){
        if(myArg.substr(0,4)=="[-s]")
            myArg=myArg.substr(4);
        else myArg=myArg.substr(2);
        if(myArg.size()==0){
            Directory& currDir = fs.getWorkingDirectory();
            string toPrint;
            currDir.sortBySize();
            for(size_t i(0);i<currDir.getChildren().size();i++){
                toPrint="";

                if(currDir.getChildren()[i]->isFile())
                    toPrint +="FILE\t";
                else toPrint +="DIR\t";

                toPrint += currDir.getChildren()[i]->getName()+"\t";
                /*toPrint += currDir.getChildren()[i]->getSize();*/

                cout<<toPrint;
                cout<<(currDir.getChildren()[i]->getSize())<<endl;
            }
        }

        else {//[-s]<path>
            Directory &currentDirToReSet = fs.getWorkingDirectory();
            CdCommand cd(myArg);
            bool didCd = cd.executeBoolLS(fs);
            if (didCd) {
                Directory &currDir = fs.getWorkingDirectory();
                string toPrint;
                currDir.sortBySize();
                for (size_t i(0); i < currDir.getChildren().size(); i++) {
                    toPrint = "";

                    if (currDir.getChildren()[i]->isFile())
                        toPrint += "FILE\t";
                    else toPrint += "DIR\t";

                    toPrint += currDir.getChildren()[i]->getName() + "\t";
                    /*toPrint += currDir.getChildren()[i]->getSize();*/

                    cout << toPrint;
                    cout<<(currDir.getChildren()[i]->getSize())<<endl;
                }
                fs.setWorkingDirectory(&currentDirToReSet);
            }
            else{
                cout << "The system cannot find the path specified" << endl;
                fs.setWorkingDirectory(toReturn);
                return;
            }
        }
    }
}

string LsCommand::toString() {
    return "ls";
}

LsCommand::~LsCommand() {}

MkdirCommand::MkdirCommand(string args):BaseCommand(args) {}

void MkdirCommand::execute(FileSystem &fs) {
    if((verbose==2) | (verbose==3)){
        cout<<this->toString()+' '+this->getArgs()<<endl;
    }

    Directory* currentDirToReSet = &fs.getWorkingDirectory();
    Directory* currentDir =  &fs.getWorkingDirectory();
    CdCommand cd(getArgs());
    if(cd.executeBool(fs)){
        cout<<"The directory already exists"<<endl;
        fs.setWorkingDirectory(currentDirToReSet);
        return;
    }
    fs.setWorkingDirectory(currentDir);

    string myArg = getArgs();

    if(myArg.size()!=0)
        myArg=myArg.substr(myArg.find_first_not_of(' '));

    if (myArg[0]=='/'){
        fs.setWorkingDirectory(&fs.getRootDirectory());
        myArg=myArg.substr(1);
        currentDir=&fs.getWorkingDirectory();
    }

    string currentName="";

    while (myArg.size()!=0){
        if((myArg[0]!='/') & (myArg[0]!='.')) {
            currentName += myArg[0];
            myArg=myArg.substr(1);
        }
        if(myArg[0]=='/'){
            bool found(false);
            for(size_t i(0);(i<currentDir->getChildren().size()) & (!found) ;i++){
                if(currentDir->getChildren()[i]->getName()==currentName ){
                    if (currentDir->getChildren()[i]->isFile()){
                        cout<<"The directory already exists"<<endl;
                        fs.setWorkingDirectory(currentDirToReSet);
                        return;
                    }
                    found=true;
                    currentDir= dynamic_cast<Directory *>(currentDir->getChildren()[i]);
                    fs.setWorkingDirectory(currentDir);
                }
            }
            if(!found){
                Directory* toAdd = new Directory(currentName,currentDir);
                currentDir->addFile(toAdd);
                fs.setWorkingDirectory(toAdd);
                currentDir=toAdd;
            }
            myArg=myArg.substr(1);
            currentName="";
        }
        if((myArg.size()>=3) && (myArg.substr(0,3)=="../")){
            if(&fs.getRootDirectory()==currentDir) {
                fs.setWorkingDirectory(currentDirToReSet);
                return;
            }
            myArg=myArg.substr(3);
            currentDir=currentDir->getParent();
            fs.setWorkingDirectory(currentDir);
        }
    }//while
    if(currentName.size()>0){//the myArg string is empty and we hold the name of the final directory
        Directory* toAdd = new Directory(currentName,currentDir);
        currentDir->addFile(toAdd);
        fs.setWorkingDirectory(toAdd);
    }
    fs.setWorkingDirectory(currentDirToReSet);
}

string MkdirCommand::toString() {
    return "mkdir";
}

MkdirCommand::~MkdirCommand() {}

/***********oren's code*************/
MkfileCommand::MkfileCommand(string args):BaseCommand(args){}

void MkfileCommand::execute(FileSystem & fs) {
    if((verbose==2) | (verbose==3)){
        cout<<this->toString()+' '+this->getArgs()<<endl;
    }

    string myArgs = getArgs();
    string currentName = "";
    Directory& toReturn=fs.getWorkingDirectory();


    size_t i(0);
    if (myArgs[0] == '/') {
        fs.setWorkingDirectory( &fs.getRootDirectory());
        i = 1;
    } else i = 0;
    for (size_t n=i; n < myArgs.size(); n = n + 1) {
        if (myArgs[n] != ' ')
        {
            if ((myArgs[n] < 'a') & (myArgs[n] > 'z') & (myArgs[n] < 'A') & (myArgs[n] > 'Z') & (myArgs[n] < '0') &
                (myArgs[n] > '9') & (myArgs[n] != '/') & (myArgs[n] != '.')) {
                cout << "The system cannot find the path specified" << endl;
                return;
            } else {
                if (myArgs[n] == '.') {
                    if (currentName != "") {
                        cout << "The system cannot find the path specified" << endl;
                        return;
                    }
                    if (myArgs.size() < n + 2) {
                        cout << "The system cannot find the path specified" << endl;
                        return;
                    }
                    if ((myArgs[n + 1] != '.') | (myArgs[n + 2] != '/')) {
                        cout << "The system cannot find the path specified" << endl;
                        return;
                    }
                    if (fs.getWorkingDirectory().getAbsolutePath() == fs.getRootDirectory().getAbsolutePath()) {
                        cout << "The system cannot find the path specified" << endl;
                        return;
                    }
                    n = n + 2;
                    fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
                } else {
                    if (myArgs[n] == '/') {

                        if ((myArgs.size() > n + 1) && (myArgs[n + 1] == '/')) {
                            cout << "The system cannot find the path specified" << endl;
                            return;
                        }
                        bool found = false;
                        for (size_t j = 0; (j < fs.getWorkingDirectory().getChildren().size()) & (!found); j = j + 1) {
                            if (fs.getWorkingDirectory().getChildren()[j]->getName() ==
                                currentName) {/////////////////////////// check if file
                                fs.setWorkingDirectory(
                                        dynamic_cast<Directory *>(fs.getWorkingDirectory().getChildren()[j]));
                                found = true;
                                currentName = "";
                            }

                        }
                        if (!found) {
                            cout << "The system cannot find the path specified" << endl;
                            fs.setWorkingDirectory(&toReturn);
                            return;
                        }

                        if ((!found)&(fs.getWorkingDirectory().getChildren().size()==0)) {
                            cout << "The system cannot find the path specified" << endl;
                            fs.setWorkingDirectory(&toReturn);
                            return;}
                    } else {
                        currentName = currentName + myArgs[n];
                    }

                }
            }
        } else{
            n=myArgs.size();
        }
    }
    myArgs=myArgs.substr(myArgs.find_first_of(' ')+1);
    int num(0);
    if((myArgs.size()>0)&&(!(myArgs==currentName))&((myArgs[0]>='0')&(myArgs[0]<='9')))
        num = stoi(myArgs);

    if (currentName.size()>0)
    {
        fs.getWorkingDirectory().addFile(new File(currentName,num));
    }
    else
    {
        cout << "The system cannot find the path specified" << endl;
    }
    fs.setWorkingDirectory(&toReturn);
    return;
}


string MkfileCommand:: toString(){
    return "mkfile";
}

MkfileCommand::~MkfileCommand() {}

CpCommand::CpCommand(string args):BaseCommand(args){}

void CpCommand:: execute(FileSystem & fs) {
    if ((verbose == 2) | (verbose == 3)) {
        cout << this->toString() + ' ' + this->getArgs() << endl;
    }

    string myArg = getArgs();
    string source = myArg.substr(0,myArg.find_first_of(' '));
    string destination = myArg.substr(myArg.find_first_of(' ')+1);

    if (source =="/")
        return;

    string currentName="";
    Directory& toReturn = fs.getWorkingDirectory();
    Directory* currDir=&fs.getWorkingDirectory();
    if(source[0]=='/') {
        *currDir = fs.getRootDirectory();
        source=source.substr(1);
    }

    while(source.size()!=0){
        if((source[0]!='.')&(source[0]!='/')){
            currentName+=source[0];
            source=source.substr(1);
        }
        if((source.size()==1)&&(source[0]=='/')&(currentName.size()>0)){
            source="";
        }

        if((source.size()>1)&&(source[0]=='/')&(currentName.size()>0)){
            bool found(false);
            for(size_t i(0);(i<currDir->getChildren().size())&(!found);i++){
                if(currDir->getChildren()[i]->getName()==currentName){
                    currDir= dynamic_cast<Directory*>(currDir->getChildren()[i]);
                    currentName="";
                    source=source.substr(1);
                    found=true;
                }
            }
            if(!found){
                cout<<"No such file or directory"<<endl;
                return;
            }
        }
        if((source.size()>2)&& (source[0]=='.')&(source[1]=='.')&(source[2]=='/')){
            if((currDir)==(&fs.getRootDirectory())){
                return;
            }
            else{
                currDir=(currDir->getParent());
                source=source.substr(3);
            }
        }
        if((source.size()>=2)&& (source[0]=='.')&(source[1]=='.')){
            if((currDir)==(&fs.getRootDirectory())){
                return;
            }
            else{
                currDir=(currDir->getParent());
                source=source.substr(2);
            }
        }

    }
    bool found(false);
    for(size_t i(0);(i<currDir->getChildren().size())&(!found);i++){
        if(currDir->getChildren()[i]->getName()==currentName){
            found=true;
            if(currDir->getChildren()[i]->isFile()){
                CdCommand cd(destination);
                if(cd.executeBoolLS(fs)){
                    bool found=false;
                    for (size_t m(0);m<fs.getWorkingDirectory().getChildren().size();m++)
                    {
                        if(fs.getWorkingDirectory().getChildren()[m]->getName()==currDir->getChildren()[i]->getName())
                            found=true;
                    }
                    if(!found)
                        fs.getWorkingDirectory().addFile(new File(currentName,currDir->getChildren()[i]->getSize()));
                }
                else{
                    cout<<"No such file or directory"<<endl;
                    return;
                }
            }
            else{//its a directory

                CdCommand cd(destination);
                if(cd.executeBoolLS(fs)){
                    bool found=false;
                    for (size_t m(0);m<fs.getWorkingDirectory().getChildren().size();m++)
                    {
                        if(fs.getWorkingDirectory().getChildren()[m]->getName()==currDir->getChildren()[i]->getName())
                            found=true;
                    }
                    if(!found)
                        fs.getWorkingDirectory().addFile(new Directory(dynamic_cast<Directory&>(*currDir->getChildren()[i])));
                }
                else{
                    cout<<"No such file or directory"<<endl;
                    return;
                }
            }
        }
    }

    fs.setWorkingDirectory(&toReturn);

}


string CpCommand:: toString(){
    return "cp";
}

CpCommand::~CpCommand() {}
/**********************end of oren's code*********/

MvCommand::MvCommand(string args):BaseCommand(args){}

void MvCommand::execute(FileSystem & fs) {
    if ((verbose == 2) | (verbose == 3)) {
        cout << this->toString() + ' ' + this->getArgs() << endl;
    }

    Directory* toReturn = &fs.getWorkingDirectory();
    string myArg = getArgs();
    string source = myArg.substr(0,myArg.find_first_of(' '));
    string destination = myArg.substr(myArg.find_first_of(' ')+1);


    if (source.size()<=destination.size()) {
        string c="";
        for (size_t i(0); i < destination.size(); i++) {
            c += destination[i];
            if (source==c) {
                fs.setWorkingDirectory(toReturn);
                return;
            }
        }
    }


    if ((source =="/")|(source ==".."))
    {
        cout<<"Can't move directory"<<endl;
        fs.setWorkingDirectory(toReturn);
        return;
    }


    string currentName="";
    Directory* currDir=&fs.getWorkingDirectory();
    if(source[0]=='/') {
        *currDir = fs.getRootDirectory();
        source=source.substr(1);
    }

    while(source.size()!=0){
        if((source[0]!='.')&(source[0]!='/')){
            currentName+=source[0];
            source=source.substr(1);
        }
        if((source.size()==1)&&(source[0]=='/')&(currentName.size()>0)){
            source="";
        }

        if((source.size()>1)&&(source[0]=='/')&(currentName.size()>0)){
            bool found(false);
            for(size_t i(0);(i<currDir->getChildren().size())&(!found);i++){
                if(currDir->getChildren()[i]->getName()==currentName){
                    currDir= dynamic_cast<Directory*>(currDir->getChildren()[i]);
                    currentName="";
                    source=source.substr(1);
                    found=true;
                }
            }
            if(!found){
                cout<<"No such file or directory"<<endl;
                fs.setWorkingDirectory(toReturn);
                return;
            }
        }
        if((source.size()>2)&& (source[0]=='.')&(source[1]=='.')&(source[2]=='/')){
            if((currDir)==(&fs.getRootDirectory())){
                return;
            }
            else{
                currDir=(currDir->getParent());
                source=source.substr(3);
            }
        }
        if((source.size()>=2)&& (source[0]=='.')&(source[1]=='.')){
            if((currDir)==(&fs.getRootDirectory())){
                return;
            }
            else{
                currDir=(currDir->getParent());
                source=source.substr(2);
            }
        }

    }

    bool found(false);
    for(size_t i(0);(i<currDir->getChildren().size())&(!found);i++){
        if(currDir->getChildren()[i]->getName()==currentName){
            found=true;
            if(currDir->getChildren()[i]->isFile()){
                CdCommand cd(destination);
                if(cd.executeBoolLS(fs)){
                    string g="";
                    for (size_t f(0);f<currDir->getAbsolutePath().size()-1;f++) {
                        g=g+currDir->getAbsolutePath()[f];
                        if ((g == currDir->getAbsolutePath())&(g!="/")) {
                            cout << "Can't move directory" << endl;
                            fs.setWorkingDirectory(toReturn);
                            return;
                        }
                    }
                    fs.getWorkingDirectory().addFile(new File(currentName,currDir->getChildren()[i]->getSize()));
                    currDir->removeFile(currDir->getChildren()[i]->getName());
                }
                else{
                    cout<<"No such file or directory"<<endl;
                    fs.setWorkingDirectory(toReturn);
                    return;
                }
            }
            else{//its a directory
                string g="";
                for (size_t f(0);f<currDir->getAbsolutePath().size()-1;f++) {
                    g=g+currDir->getAbsolutePath()[f];
                    if ((g == currDir->getAbsolutePath())&(g!="/")) {
                        cout << "Can't move directory" << endl;
                        fs.setWorkingDirectory(toReturn);
                        return;
                    }
                }

                CdCommand cd(destination);
                if(cd.executeBoolLS(fs)){
                    (dynamic_cast<Directory*>(currDir->getChildren()[i]))->setParent(&fs.getWorkingDirectory());
                }
                else{
                    cout<<"No such file or directory"<<endl;
                    fs.setWorkingDirectory(toReturn);
                    return;
                }
            }
        }
    }
    if (!found) {
        cout << "No such file or directory" << endl;
        return;
    }
    fs.setWorkingDirectory(toReturn);

}

string MvCommand:: toString(){
    return "mv";
}

MvCommand::~MvCommand() {}

RenameCommand::RenameCommand(string args):BaseCommand(args) {}

void RenameCommand::execute(FileSystem &fs) {
    if((verbose==2) | (verbose==3)){
        cout<<this->toString()+' '+this->getArgs()<<endl;
    }

    string source = "";
    string newName = "";
    string myArg = getArgs();
    myArg = myArg.substr(myArg.find_first_not_of(' '));

    while (myArg[0] != ' ') {
        source += myArg[0];
        myArg = myArg.substr(1);
    }
    if (myArg[0] == ' ')
        myArg = myArg.substr(1);

    while (myArg.size() > 0) {
        newName += myArg[0];
        myArg = myArg.substr(1);
    }

    string workingDirName = fs.getWorkingDirectory().getName();
    Directory* currDir = &fs.getWorkingDirectory();
    if ((source[0] == '/') & (source.size() > 1)) {
        *currDir = fs.getRootDirectory();
        source = source.substr(1);
    }
    string origin=fs.getWorkingDirectory().getAbsolutePath();

    string currentName = "";
    bool found(false);

    while (!found) {
        if ((source[0] != '.') & (source[0] != '/')) {
            currentName += source[0];
            source = source.substr(1);
        }

        if ((source.size() == 0) & (currentName.size() > 0)) {
            if (currentName == workingDirName) {
                cout << "Can’t rename the working directory" << endl;
                return;
            }
            size_t counter = 0;
            size_t pos = -1;
            for (size_t i(0); i < currDir->getChildren().size(); i++) {
                if (currDir->getChildren()[i]->getName() == currentName) {
                    counter++;
                    pos = i;
                }
            }

            for (size_t i(0); i < currDir->getChildren().size(); i++){
                if (currDir->getChildren()[i]->getName() == newName)
                    return;
            }

            if (counter == 1) {
                if (currDir->getAbsolutePath()==origin)
                {cout<<"Can’t rename the working directory"<<endl;
                    return;
                }
                currDir->getChildren()[pos]->setName(newName);
                found = true;
                return;

            } else if (counter > 1)
                return;

            else {
                cout << "No such file or directory" << endl;
                return;
            }
        }

        if ((source[0] == '/') & (currentName.size() > 0)) {
            bool stop(false);
            for (size_t i(0); (i<currDir->getChildren().size()) & (!stop); i++) {
                if (currDir->getChildren()[i]->getName() == currentName) {
                    if(currDir->getChildren()[i]->isFile()){
                        cout<<"No such file or directory"<<endl;
                        return;}
                    stop=true;
                    currDir=(dynamic_cast<Directory*>(currDir->getChildren()[i]));
                    source=source.substr(1);
                    currentName="";
                }
            }
            if(!stop){
                cout << "No such file or directory" << endl;
                return;
            }
        }

        if((source.size()>=3) & (source[0]=='.') & (source[1]=='.') & (source[2]=='/')){
            if(currDir->getAbsolutePath()==fs.getRootDirectory().getAbsolutePath()){
                cout<<"No such file or directory"<<endl;
                return;
            }
            currDir=currDir->getParent();
            source=source.substr(3);
        }
    }
}

string RenameCommand::toString() {
    return "rename";
}

RenameCommand::~RenameCommand() {}

RmCommand::RmCommand(string args):BaseCommand(args) {}

void RmCommand::execute(FileSystem &fs) {
    if((verbose==2) | (verbose==3)){
        cout<<this->toString()+' '+this->getArgs()<<endl;
    }

    string path=getArgs();
    Directory& workingDir = fs.getWorkingDirectory();

    if((path.size()==1)&&(path[0]=='/')){
        cout<<"Can't remove directory"<<endl;
        return;
    }

    if((path[0]=='/') & (path.size()>1)){
        fs.setWorkingDirectory(&fs.getRootDirectory());
        path=path.substr(1);
    }

    Directory* currDir=&fs.getWorkingDirectory();

    bool found(false);
    string currentName="";
    while(!found){
        if((path[0]!='/') & (path[0]!='.')){
            currentName+=path[0];
            path=path.substr(1);
        }

        if((path[0]=='/')& (path.size()>1) & (currentName.size()>0)){
            bool stop(false);
            for (size_t i(0);(i< currDir->getChildren().size()) & (!stop); i++) {
                if (currDir->getChildren()[i]->getName() == currentName) {
                    currDir = ((dynamic_cast<Directory*>(currDir->getChildren()[i])));
                    path=path.substr(1);
                    currentName="";
                    stop=true;
                }
            }
            if(!stop){
                cout<<"No such file or directory"<<endl;
                return;
            }
        }

        if((path.size()==0) & (currentName.size()>0)){
            bool stop(false);
            for (size_t i=0;(i<currDir->getChildren().size()) &(!stop);i++) {
                if((currDir->getChildren()[i])->getName() == currentName){
                    if(currDir->getChildren()[i] != &fs.getWorkingDirectory()) {
                        (currDir->removeFile(currDir->getChildren()[i]));
                        stop = true;
                        found=true;
                    }
                    else{
                        cout<<"Can't remove directory"<<endl;
                        return;
                    }
                }
            }
            if(!stop){
                cout<<"No such file or directory"<<endl;
                return;
            }
            fs.setWorkingDirectory(&workingDir);
        }

        if ((path.size()>=3) && (path[0]=='.')&(path[1]=='.')&(path[2]=='/')){
            if(currDir == &fs.getRootDirectory()){
                return;
            }
            currDir=currDir->getParent();
            path=path.substr(3);
        }
        else if((path.size()>=2)&&(path[0]=='.')&(path[1]=='.')){
            if(currDir == &fs.getRootDirectory()){
                return;
            }
            currDir=currDir->getParent();
            path=path.substr(2);
        }
    }
}

string RmCommand::toString() {
    return "rm";
}

RmCommand::~RmCommand() {}

HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> & history):BaseCommand(args),history(history){}

void HistoryCommand::execute(FileSystem &fs) {
    if((verbose==2) | (verbose==3)){
        if(this->getArgs().size()>0)
            cout<<this->toString()+' '+this->getArgs()<<endl;
        else
            cout<<this->toString()<<endl;
    }

    string currentCommandToPrint="";
    int counter(0);
    for (auto i=history.begin() ; i!= history.end() ; i++){
        cout<<counter;
        if(i.operator*()->toString() != "error")
            currentCommandToPrint+="\t"+((i.operator*())->toString());
        string args = i.operator*()->getArgs();

        if(i.operator*()->getArgs().length()>0) {
            if(i.operator*()->toString() == "error"){}
            currentCommandToPrint+="\t";
            currentCommandToPrint += ' ' + ((i.operator*())->getArgs());
        }
        cout<<currentCommandToPrint<<endl;

        currentCommandToPrint="";
        counter++;
    }
    return;
}

string HistoryCommand::toString() {
    return "history";
}

HistoryCommand::~HistoryCommand() {}

VerboseCommand::VerboseCommand(string args):BaseCommand(args){}

void VerboseCommand::execute(FileSystem &fs) {
    string myArg=getArgs();
    myArg=myArg.substr(myArg.find_first_not_of(' '));

    if(myArg == "0")
        verbose=0;
    else if(myArg== "1")
        verbose=1;
    else if(myArg=="2")
        verbose=2;
    else if (myArg=="3")
        verbose=3;
    else{
        cout<<"Wrong verbose input"<<endl;
        return;
    }
}

string VerboseCommand::toString() {
    return "verbose";
}

VerboseCommand::~VerboseCommand() {}

ErrorCommand::ErrorCommand(string args):BaseCommand(args) {

}

void ErrorCommand::execute(FileSystem & fs) {
    if((verbose==2) | (verbose==3)){
        cout<<this->getArgs()<<endl;
    }
    string toPr=getArgs().substr(0,getArgs().find_first_of(' '));
    cout<<toPr+": Unknown command"<<endl;
}

string ErrorCommand::toString() {
    return "error";
}

ErrorCommand::~ErrorCommand() {}

ExecCommand::ExecCommand(string args, const vector<BaseCommand *> & history):BaseCommand(args),history(history) {

}

void ExecCommand::execute(FileSystem & fs)
{
    if ((verbose==2)|(verbose==3)) {
        cout << this->toString() + ' ' + this->getArgs() << endl;
    }
    string myArg=getArgs();
    int c = stoi(myArg);

    if ((size_t)c>history.size())
        cout<<"Command not found"<<endl;

    else
    {
        string st=history[c]->getArgs();
        string commander=history[c]->toString();
        if (commander=="pwd")
        {
            PwdCommand pwd(st);
            pwd.execute(fs);
        }else
        if (commander=="cd")
        {
            CdCommand cd(st);
            cd.execute(fs);
        }else
        if (commander=="ls")
        {
            LsCommand ls(st);
            ls.execute(fs);
        }else
        if (commander=="mkdir")
        {
            MkdirCommand mkdir(st);
            mkdir.execute(fs);
        }else
        if (commander=="mkfile")
        {
            MkfileCommand mkfile(st);
            mkfile.execute(fs);
        }else
        if (commander=="cp")
        {
            CpCommand cp(st);
            cp.execute(fs);
        }else
        if (commander=="mv")
        {
            MvCommand mv(st);
            mv.execute(fs);
        }else
        if (commander=="rename")
        {
            RenameCommand rn(st);
            rn.execute(fs);
        }else
        if (commander=="rm")
        {
            RmCommand rm(st);
            rm.execute(fs);
        }else
        if (commander=="verbose")
        {
            VerboseCommand ver(st);
            ver.execute(fs);
        }else
        if (commander=="history")
        {
            HistoryCommand histoy(st,history);
            histoy.execute(fs);
        }else
        if (commander=="exec")
        {
            ExecCommand ex(st,history);
            ex.execute(fs);
        }else
        {
            ErrorCommand er(commander);
            er.execute(fs);
        }

    }
}

string ExecCommand::toString() {
    return "exec";
}

ExecCommand::~ExecCommand() {}