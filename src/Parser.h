#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

using namespace std;

struct Command {
    vector<string> args;
    string inputFile;
    string outputFile;
    bool appendOutput;
    bool hasPipe;
    Command* pipeCmd;
    bool background;
    
    Command() : appendOutput(false), hasPipe(false), 
                pipeCmd(nullptr), background(false) {}
    
    ~Command() {
        if (pipeCmd) {
            delete pipeCmd;
        }
    }
};

class Parser {
private:
    string line;
    vector<string> tokenize(const string& str);
    Command parseSimple(const vector<string>& tokens);

public:
    Parser(const string& commandLine);
    ~Parser();
    Command parse();
};

#endif
