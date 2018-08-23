#include "../include/Environment.h"
#include "../include/GlobalVariables.h"
#include <iostream>
#include <algorithm>//
using namespace std;

Environment::Environment():commandsHistory(),fs(){

}

void Environment::start(){
    string s="";
    string commander="";
    cout << "/>";
    getline(cin, s);
    while(s!="exit") {

        bool found =false;
        for (size_t i(0);(i<s.size())&(!found);i++)
        {
            if (s[i]==' ')
            {
                found= true;
                s.erase(0,i+1);
            } else
                commander= commander+s[i];
        }
        if(!found)
            s = "";

        if (commander=="pwd")
        {
            PwdCommand* pwd = new PwdCommand(s);
            addToHistory(pwd);
            (*pwd).execute(fs);
        }
        else if (commander=="cd")
        {
            CdCommand* cd = new CdCommand(s);
            addToHistory(cd);
            (*cd).execute(fs);
        }
        else if (commander=="ls")
        {
            LsCommand* ls = new LsCommand(s);
            addToHistory(ls);
            (*ls).execute(fs);
        }
        else if (commander=="mkdir")
        {
            MkdirCommand* mkdir =new MkdirCommand(s);
            addToHistory(mkdir);
            (*mkdir).execute(fs);
        }
        else if (commander=="mkfile")
        {
            MkfileCommand* mkfile = new MkfileCommand(s);
            addToHistory(mkfile);
            (*mkfile).execute(fs);
        }
        else if (commander=="cp")
        {
            CpCommand* cp=new CpCommand(s);
            addToHistory(cp);
            (*cp).execute(fs);
        }
        else if (commander=="mv")
        {
            MvCommand* mv=new MvCommand(s);
            addToHistory(mv);
            (*mv).execute(fs);
        }
        else if(commander=="rename"){
            RenameCommand* rename=new RenameCommand(s);
            addToHistory(rename);
            (*rename).execute(fs);
        }
        else if(commander=="rm"){
            RmCommand* rm=new RmCommand(s);
            addToHistory(rm);
            (*rm).execute(fs);
        }
        else if(commander=="history"){
            HistoryCommand* hist = new HistoryCommand(s,commandsHistory);
            (*hist).execute(fs);
            addToHistory(hist);
        }
        else if(commander=="verbose"){
            VerboseCommand* ver=new VerboseCommand(s);
            addToHistory(ver);
            (*ver).execute(fs);
        }
        else if(commander=="exec"){
            ExecCommand* exec=new ExecCommand(s,commandsHistory);
            addToHistory(exec);
            (*exec).execute(fs);
        }

        else{
            commander=commander+' '+s;
            ErrorCommand* er=new ErrorCommand(commander);
            addToHistory(er);
            (*er).execute(fs);
        }

        s="";
        commander="";
        string tmp = fs.getWorkingDirectory().getAbsolutePath();
        if(tmp.size()>1)
            tmp=tmp.substr(1);
        cout <<tmp+">";
        getline(cin, s);
    }
    return;
}

FileSystem& Environment::getFileSystem(){
    return (FileSystem &)fs;
}

void Environment::addToHistory(BaseCommand *command){
    commandsHistory.push_back(command);
}

const vector<BaseCommand*>& Environment::getHistory() const{
    return  commandsHistory;
}

//rule of 5 functions

Environment::~Environment(){
    if((verbose==1) | (verbose==3)){
        cout<<"Environment::~Environment()"<<endl;
    }
    clean();
}

void Environment::clean() {
    while (commandsHistory.size()!=0){
        delete(commandsHistory[0]);
        commandsHistory.erase(commandsHistory.begin());
    }

}

Environment::Environment(const Environment &other):commandsHistory(),fs(other.fs){
    if((verbose==1) | (verbose==3)){
        cout<<"Environment::Environment(const Environment &other)"<<endl;
    }
    copy(other);
}

void Environment::copy(const Environment &other) {
    for(size_t i=0;i<other.commandsHistory.size();i++){
        string commander = other.commandsHistory[i]->toString();
        string s= other.commandsHistory[i]->getArgs();

        if (commander=="pwd")
        {
            PwdCommand pwd(s);
            commandsHistory.push_back(&pwd);
        }
        else if (commander=="cd")
        {
            CdCommand cd(s);
            commandsHistory.push_back(&cd);
        }
        else if (commander=="ls")
        {
            LsCommand ls(s);
            commandsHistory.push_back(&ls);
        }
        else if (commander=="mkdir")
        {
            MkdirCommand mkdir(s);
            commandsHistory.push_back(&mkdir);
        }
        else if (commander=="cd")
        {
            MkfileCommand mkfile(s);
            commandsHistory.push_back(&mkfile);
        }
        else if (commander=="cp")
        {
            CpCommand cp(s);
            commandsHistory.push_back(&cp);
        }
        else if (commander=="mv")
        {
            MvCommand mv(s);
            commandsHistory.push_back(&mv);
        }
        else if(commander=="rename"){
            RenameCommand rename(s);
            commandsHistory.push_back(&rename);
        }
        else if(commander=="rm"){
            RmCommand rm(s);
            commandsHistory.push_back(&rm);
        }
        else if(commander=="history"){
            HistoryCommand hist(s,commandsHistory);
            commandsHistory.push_back(&hist);
        }
        else if(commander=="verbose"){
            VerboseCommand ver(s);
            commandsHistory.push_back(&ver);
        }
        else if(commander=="exec"){
            ExecCommand exec(s,commandsHistory);
            commandsHistory.push_back(&exec);
        }

        else{
            ErrorCommand er(commander);
            commandsHistory.push_back(&er);
        }
    }
}

Environment& Environment::operator=(const Environment& other){
    if((verbose==1) | (verbose==3)){
        cout<<"Environment& Environment::operator=(const Environment& other)"<<endl;
    }
    if(this!=&other) {
        clean();
        copy(other);
    }
    return *this;
}

Environment::Environment(Environment &&other):commandsHistory(),fs(){
    if((verbose==1) | (verbose==3)){
        cout<<"Environment::Environment(Environment &&other)"<<endl;
    }
    steal(other);
}

void Environment::steal(Environment &other) {
    fs.setWorkingDirectory(&other.fs.getWorkingDirectory());
    commandsHistory=other.getHistory();

    delete &other.fs;
    delete &commandsHistory;
}

Environment& Environment::operator=(Environment &&other){
    if((verbose==1) | (verbose==3)){
        cout<<"Environment& Environment::operator=(Environment &&other)"<<endl;
    }

    clean();
    steal(other);
    return *this;
}