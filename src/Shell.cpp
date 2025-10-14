#include "Shell.h"
#include "Parser.h"
#include "Executor.h"
#include "Builtin.h"
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

void Shell::processCommand(const string& line) {
    if (line.empty()) return;
    
    addToHistory(line);
    
    Parser parser(line);
    Command cmd = parser.parse();
    
    if (!cmd.args.empty()) {
        string aliasValue = getAlias(cmd.args[0]);
        if (!aliasValue.empty()) {
            Parser aliasParser(aliasValue);
            cmd = aliasParser.parse();
        }
    }
    
    if (!cmd.args.empty()) {
        string builtinCmd = cmd.args[0];
        
        if (builtinCmd == "salir" || builtinCmd == "exit") {
            running = false;
            return;
        }
        else if (builtinCmd == "cd") {
            Builtin::cd(cmd.args);
            return;
        }
        else if (builtinCmd == "pwd") {
            Builtin::pwd();
            return;
        }
        else if (builtinCmd == "help") {
            Builtin::help();
            return;
        }
        else if (builtinCmd == "history") {
            Builtin::history(history);
            return;
        }
        else if (builtinCmd == "alias") {
            if (cmd.args.size() >= 3 && cmd.args[2] == "=") {
                string name = cmd.args[1];
                string value = "";
                for (size_t i = 3; i < cmd.args.size(); i++) {
                    value += cmd.args[i];
                    if (i < cmd.args.size() - 1) value += " ";
                }
                setAlias(name, value);
                cout << "Alias creado: " << name << " = " << value << endl;
            } else {
                cout << "Uso: alias nombre = comando" << endl;
            }
            return;
        }
    }
    
    Executor executor;
    executor.execute(cmd);
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
	
	try {
            processCommand(line);
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}
