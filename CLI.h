#ifndef MY_PROJECT_CLI_H
#define MY_PROJECT_CLI_H

#include "string"

using namespace std;

class CLI {
public:
    void start();

};

class DefaultIO {
public:
    virtual string read() = 0;

    virtual void write(string s) = 0;
};

class Command {
private:
    string description;
    DefaultIO *dio;
public:
    virtual void execute() = 0;

    void printDescription();
};

#endif //MY_PROJECT_CLI_H
