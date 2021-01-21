
#include "Server.h"
#include <signal.h>

#define QUEUE 3


Server::Server(int port) throw(const char *) {
    fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);   // socket
    if (fileDescriptor < 0) {
        throw "socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(fileDescriptor, (struct sockaddr *) &server, sizeof(server)) < 0) { // bind server and port
        close(fileDescriptor);
        throw "bind failed";
    }
    if (listen(fileDescriptor, QUEUE) < 0) { // listen - waiting for a client
        close(fileDescriptor);
        throw "listen failed";
    }

}

void helper(int signum) {
    signal(SIGALRM,helper);
}

void Server::start(ClientHandler &ch) throw(const char *) {
    t = new thread([&ch, this]() {
        signal(SIGALRM, helper);
        while (!toStop) {
            socklen_t clientSize = sizeof(client);
            alarm(5); // set alarm to 1 seconds
            int aClient = accept(fileDescriptor, (struct sockaddr *) &client, &clientSize); //accept a new client
            alarm(0);
            if (aClient < 0) {
                close(fileDescriptor);
                throw "accept failed";
            }
            ch.handle(aClient);
            close(aClient);
        }
    });
}

void Server::stop() {
    toStop = true;
    close(fileDescriptor);
    t->join(); // do not delete this!
}

Server::~Server() {
}

void AnomalyDetectionHandler::handle(int clientID) {
    DefaultIO *dio = new SocketIO(clientID);
    CLI cli(dio);
    cli.start();
    free(dio);
}
