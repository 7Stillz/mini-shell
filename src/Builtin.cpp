#include "Builtin.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cerrno>

using namespace std;

void Builtin::cd(const vector<string>& args) {
    string targetDir;
    
    if (args.size() < 2) {
        const char* home = getenv("HOME");
        if (home) {
            targetDir = home;
        } else {
            cerr << "cd: No se pudo obtener el directorio HOME" << endl;
            return;
        }
    } else {
        targetDir = args[1];
    }
    
    if (chdir(targetDir.c_str()) != 0) {
        cerr << "cd: " << strerror(errno) << ": " << targetDir << endl;
    }
}

void Builtin::pwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        cout << cwd << endl;
    } else {
        perror("pwd");
    }
}

void Builtin::help() {
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║              MINI-SHELL - AYUDA                            ║\n";
    cout << "╠════════════════════════════════════════════════════════════╣\n";
    cout << "║                                                            ║\n";
    cout << "║ COMANDOS INTERNOS:                                         ║\n";
    cout << "║   cd [dir]        - Cambiar de directorio                  ║\n";
    cout << "║   pwd             - Mostrar directorio actual              ║\n";
    cout << "║   help            - Mostrar esta ayuda                     ║\n";
    cout << "║   history         - Mostrar historial de comandos          ║\n";
    cout << "║   alias n = cmd   - Crear alias para comando               ║\n";
    cout << "║   salir / exit    - Salir de la shell                      ║\n";
    cout << "║                                                            ║\n";
    cout << "║ REDIRECCIONES:                                             ║\n";
    cout << "║   cmd > file      - Redirigir salida (truncar)             ║\n";
    cout << "║   cmd >> file     - Redirigir salida (agregar)             ║\n";
    cout << "║   cmd < file      - Redirigir entrada                      ║\n";
    cout << "║                                                            ║\n";
    cout << "║ PIPES:                                                     ║\n";
    cout << "║   cmd1 | cmd2     - Conectar salida de cmd1 a cmd2         ║\n";
    cout << "║                                                            ║\n";
    cout << "║ EJECUCION EN SEGUNDO PLANO:                                ║\n";
    cout << "║   cmd &           - Ejecutar en background                 ║\n";
    cout << "║                                                            ║\n";
    cout << "║ EJEMPLOS:                                                  ║\n";
    cout << "║   ls -la                                                   ║\n";
    cout << "║   cat archivo.txt > salida.txt                             ║\n";
    cout << "║   ls | grep txt                                            ║\n";
    cout << "║   sleep 10 &                                               ║\n";
    cout << "║                                                            ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n\n";
}

void Builtin::history(const vector<string>& hist) {
    if (hist.empty()) {
        cout << "Historial vacio" << endl;
        return;
    }
    
    cout << "\nHistorial de comandos:\n";
    cout << "────────────────────────────────\n";
    for (size_t i = 0; i < hist.size(); i++) {
        cout << "  " << (i + 1) << ". " << hist[i] << endl;
    }
    cout << endl;
}
