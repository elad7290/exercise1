#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "CLI.h"


using namespace std;

// ClientHandler interface:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;
};

class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) override;
};

class Server {
private:
    thread *t; // the thread to run the start() method in
    sockaddr_in server;
    sockaddr_in client;
    int fileDescriptor;
    bool toStop = false;
public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};

#endif /* SERVER_H_ */
