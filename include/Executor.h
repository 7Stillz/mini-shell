#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "Parser.h"
#include <string>

using namespace std;

class Executor {
private:
    string resolvePath(const string& cmd);
    void executeSimple(const Command& cmd);
    void executePipe(const Command& cmd);
    void setupRedirections(const Command& cmd);

public:
    Executor();
    ~Executor();
    void execute(const Command& cmd);
};

#endif
