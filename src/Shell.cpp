#include "Shell.h"
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <cstring>

using namespace std;

Shell::Shell() : running(true) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        currentDir = string(cwd);
    }
}

Shell::~Shell() {}

string Shell::getPrompt() {
    char cwd[1024];
    string prompt = "\033[1;36m";  // Color cyan bold
    
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        string path(cwd);
        size_t pos = path.find_last_of('/');
        if (pos != string::npos) {
            path = path.substr(pos + 1);  // Solo nombre directorio
        }
        prompt += path;
    }
    
    prompt += "\033[0m $ ";  // Reset color + " $ "
    return prompt;
}

void Shell::addToHistory(const string& cmd) {
    if (!cmd.empty() && (history.empty() || history.back() != cmd)) {
        history.push_back(cmd);
    }
}

void Shell::setAlias(const string& name, const string& value) {
    aliases[name] = value;
}

string Shell::getAlias(const string& name) {
    // Busca el 'name' (nombre del alias) en el contenedor de 'aliases'.
    auto it = aliases.find(name);
    // Si el iterador devuelto no es el final entonces 'name' fue encontrado.
    if (it != aliases.end()) {
        return it->second;
    }
    // Si el 'name' no se encuentra, devuelve una cadena vacía.
    return "";
}

void Shell::run() {
    char* input;
    
    while (running) {
        string prompt = getPrompt();
        input = readline(prompt.c_str()); // Entrada con readline (soporta flechas ↑/↓) 
        
        if (!input) { // Ctrl+D
            cout << endl;
            break;
        }
        
        string line(input);
        
        if (!line.empty()) {
            add_history(input); // Agregar a historial readline
        }
        
        free(input); // Liberar memoria readline
    }
}
