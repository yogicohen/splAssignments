#include <stdlib.h>
#include <boost/thread.hpp>
#include "../include/connectionHandler.h"
#include "Task1.cpp"
#include "Task2.cpp"


int main (int argc, char *argv[]) {
    if (argc < 3) {
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    
    ConnectionHandler connectionHandler(host, port);
    std::atomic<bool> isLoggedIn(false);
    boost::mutex mutex;

    Task1 task1(&mutex,&connectionHandler,&isLoggedIn);
    Task2 task2(&mutex,&connectionHandler,&isLoggedIn);

    boost::thread th1(task1);
    boost::thread th2(task2);

    th1.join();
    th2.join();


    return 0;
}
