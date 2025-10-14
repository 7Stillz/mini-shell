#ifndef BUILTIN_H
#define BUILTIN_H

#include <string>
#include <vector>

using namespace std;

class Builtin {
public:
    static void cd(const vector<string>& args);
    static void pwd();
    static void help();
    static void history(const vector<string>& hist);
};

#endif
