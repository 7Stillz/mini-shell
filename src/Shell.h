#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class Shell {
private:
    bool running;
    vector<string> history;
    map<string, string> aliases;
    string currentDir;
    
    string getPrompt();
    void addToHistory(const string& cmd);

public:
    Shell();
    ~Shell();
    
    void run();
    const vector<string>& getHistory() const { return history; }
    void setAlias(const string& name, const string& value);
    string getAlias(const string& name);
};

#endif

