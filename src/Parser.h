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
        if (pipeCmd != nullptr) {
            delete pipeCmd;
	    pipeCmd = nullptr;
        }
    }
    
    Command(const Command& other) 
        : args(other.args),
          inputFile(other.inputFile),
          outputFile(other.outputFile),
          appendOutput(other.appendOutput),
          hasPipe(other.hasPipe),
          pipeCmd(nullptr),
          background(other.background) {
        
        if (other.pipeCmd != nullptr) {
            pipeCmd = new Command(*other.pipeCmd);
        }
    }

    Command& operator=(const Command& other) {
        if (this != &other) {
            args = other.args;
            inputFile = other.inputFile;
            outputFile = other.outputFile;
            appendOutput = other.appendOutput;
            hasPipe = other.hasPipe;
            background = other.background;
            
            if (pipeCmd != nullptr) {
                delete pipeCmd;
            }
            
            if (other.pipeCmd != nullptr) {
                pipeCmd = new Command(*other.pipeCmd);
            } else {
                pipeCmd = nullptr;
            }
        }
        return *this;
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
