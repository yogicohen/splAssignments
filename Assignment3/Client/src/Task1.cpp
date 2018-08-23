#include <stdlib.h>
#include <boost/thread.hpp>
#include "../include/connectionHandler.h"

class Task1 {
private:
    boost::mutex *_mutex;
    ConnectionHandler *_connectionHandler;
    std::atomic<bool> *_isLoggedIn;
public:
    Task1(boost::mutex* mutex,ConnectionHandler *connectionHandler,std::atomic<bool> *isLoggedIn)
            :_mutex(mutex),_connectionHandler(connectionHandler),_isLoggedIn(isLoggedIn) {}

    void operator()(){
        while(!std::cin.eof()) {
            const short bufsize = 1024;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);
            std::string line(buf);
            if (! _connectionHandler->sendLine(line)) {
                boost::mutex::scoped_lock lock(*_mutex);
                //_stop=true;
            }
            if(_isLoggedIn->load() && ((line == "SIGNOUT")|(line =="signout")))
                break;
        }
    }
};

