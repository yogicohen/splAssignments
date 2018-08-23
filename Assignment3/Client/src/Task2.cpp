#include <stdlib.h>
#include <boost/thread.hpp>
#include "../include/connectionHandler.h"

class Task2 {
private:
    boost::mutex *_mutex;
    ConnectionHandler *_connectionHandler;
    std::atomic<bool> *_isLoggedIn;
public:
    Task2 (boost::mutex* mutex,ConnectionHandler *connectionHandler,std::atomic<bool> *isLoggedIn)
            :_mutex(mutex),_connectionHandler(connectionHandler),_isLoggedIn(isLoggedIn) {}

    void operator()(){

        if (!_connectionHandler->connect()) {
            std::cerr << "Cannot connect to "  ": Server" << std::endl;
            return;
        }
        while (1) {
            std::string answer;
            if (!_connectionHandler->getLine(answer)) {
                boost::mutex::scoped_lock lock(*_mutex);
                break;
            }

            int len2=answer.length();

            answer.resize(len2-1);

            boost::mutex::scoped_lock lock(*_mutex);
            std::cout << answer << std::endl;

            if(answer == "ACK login succeeded"){
                _isLoggedIn->exchange(true);
            }

            if (answer == "ACK signout succeeded") {
                _connectionHandler->close();
                break;
            }
        }
    }
};