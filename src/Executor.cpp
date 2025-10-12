#include "Executor.h"      
#include <iostream>        
#include <unistd.h>        
#include <sys/wait.h>      
#include <sys/types.h>     
#include <sys/stat.h>      
#include <fcntl.h>         
#include <cstring>         
#include <cerrno>          

using namespace std;

Executor::Executor() {}
Executor::~Executor() {}

string Executor::resolvePath(const string& cmd) {
    // Si el comando ya es una ruta absoluta (empieza con /), la devuelve.
    if (cmd[0] == '/') {
        return cmd;
    }
    
    // Si es una ruta relativa (empieza con ./ o ../), la devuelve.
    if (cmd.substr(0, 2) == "./" || cmd.substr(0, 3) == "../") {
        return cmd;
    }
    
    // Busca en /bin. Si existe y es ejecutable, devuelve esa ruta.
    string binPath = "/bin/" + cmd;
    if (access(binPath.c_str(), X_OK) == 0) {
        return binPath;
    }
    
    // Busca en /usr/bin. Si existe y es ejecutable, devuelve esa ruta.
    string usrBinPath = "/usr/bin/" + cmd;
    if (access(usrBinPath.c_str(), X_OK) == 0) {
        return usrBinPath;
    }
    
    // Si no se encontro en ninguna ruta, devuelve el comando original.
    return cmd;
}

void Executor::setupRedirections(const Command& cmd) {
    // Redireccion de entrada (< fichero)
    if (!cmd.inputFile.empty()) {
        int fd = open(cmd.inputFile.c_str(), O_RDONLY);
        if (fd < 0) {
            perror(("Error al abrir " + cmd.inputFile).c_str());
            exit(1); // Termina el proceso hijo si hay error
        }
        dup2(fd, STDIN_FILENO); // Redirige la entrada estandar (teclado) al archivo
        close(fd);
    }
    
    // Redireccion de salida (> o >> fichero)
    if (!cmd.outputFile.empty()) {
        int flags = O_WRONLY | O_CREAT; // Banderas base: escribir y crear si no existe
        if (cmd.appendOutput) {
            flags |= O_APPEND; // Añadir al final (>>)
        } else {
            flags |= O_TRUNC; // Sobrescribir (>)
        }
        
        int fd = open(cmd.outputFile.c_str(), flags, 0644); // 0644 son los permisos del archivo
        if (fd < 0) {
            perror(("Error al crear " + cmd.outputFile).c_str());
            exit(1); // Termina el proceso hijo si hay error
        }
        dup2(fd, STDOUT_FILENO); // Redirige la salida estandar (pantalla) al archivo
        close(fd);
    }
}

void Executor::executeSimple(const Command& cmd) {
    if (cmd.args.empty()) {
        return; // No hace nada si no hay comando
    }
    
    string path = resolvePath(cmd.args[0]); // Obtiene la ruta del ejecutable
    
    pid_t pid = fork(); // Crea un nuevo proceso
    
    if (pid < 0) {
        perror("Error en fork");
        return;
    }
    else if (pid == 0) { // --- Codigo del Proceso Hijo ---
        setupRedirections(cmd); // Configura redirecciones si las hay
        
        // Prepara el array de argumentos para execvp (debe terminar en nullptr)
        char** argv = new char*[cmd.args.size() + 1];
        for (size_t i = 0; i < cmd.args.size(); i++) {
            argv[i] = const_cast<char*>(cmd.args[i].c_str());
        }
        argv[cmd.args.size()] = nullptr;
        
        // Reemplaza el proceso hijo con el nuevo comando
        execvp(path.c_str(), argv);
        
        // Si execvp retorna, es porque hubo un error
        cerr << "Error: No se pudo ejecutar '" << cmd.args[0] << "': " 
             << strerror(errno) << endl;
        exit(1);
    }
    else { // --- Codigo del Proceso Padre ---
        if (!cmd.background) {
            // Si no es un proceso en segundo plano, espera a que el hijo termine
            int status;
            waitpid(pid, &status, 0);
        } else {
            // Si es en segundo plano, solo informa el PID y continua
            cout << "[Background] PID: " << pid << endl;
        }
    }
}

void Executor::executePipe(const Command& cmd) {
    // Si no hay pipe, ejecuta como un comando simple
    if (!cmd.hasPipe || !cmd.pipeCmd) {
        executeSimple(cmd);
        return;
    }
    
    int pipefd[2]; // pipefd[0] es para leer, pipefd[1] es para escribir
    if (pipe(pipefd) < 0) {
        perror("Error al crear pipe");
        return;
    }
    
    // --- Primer Proceso Hijo (comando a la izquierda de |) ---
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("Error en fork (primer comando)");
        return;
    }
    else if (pid1 == 0) {
        close(pipefd[0]); // Cierra el extremo de lectura del pipe, no lo necesita
        dup2(pipefd[1], STDOUT_FILENO); // Redirige su salida estandar para que escriba en el pipe
        close(pipefd[1]); // Cierra el descriptor original
        
        // Tambien maneja la redireccion de entrada si la tuviera (ej: cat file.txt | grep ...)
        if (!cmd.inputFile.empty()) {
            int fd = open(cmd.inputFile.c_str(), O_RDONLY);
            if (fd < 0) { perror("Error al abrir entrada"); exit(1); }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        
        // Ejecuta el primer comando
        string path = resolvePath(cmd.args[0]);
        char** argv = new char*[cmd.args.size() + 1];
        for (size_t i = 0; i < cmd.args.size(); i++) {
            argv[i] = const_cast<char*>(cmd.args[i].c_str());
        }
        argv[cmd.args.size()] = nullptr;
        execvp(path.c_str(), argv);
        
        // Si execvp falla
        cerr << "Error: No se pudo ejecutar '" << cmd.args[0] << "': " << strerror(errno) << endl;
        exit(1);
    }
    
    // --- Segundo Proceso Hijo (comando a la derecha de |) ---
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("Error en fork (segundo comando)");
        return;
    }
    else if (pid2 == 0) {
        close(pipefd[1]); // Cierra el extremo de escritura del pipe, no lo necesita
        dup2(pipefd[0], STDIN_FILENO); // Redirige su entrada estandar para que lea del pipe
        close(pipefd[0]); // Cierra el descriptor original
        
        // Tambien maneja la redireccion de salida si la tuviera (ej: ... | grep ... > out.txt)
        if (!cmd.pipeCmd->outputFile.empty()) {
            int flags = O_WRONLY | O_CREAT;
            if (cmd.pipeCmd->appendOutput) flags |= O_APPEND;
            else flags |= O_TRUNC;
            
            int fd = open(cmd.pipeCmd->outputFile.c_str(), flags, 0644);
            if (fd < 0) { perror("Error al crear archivo de salida"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        
        // Ejecuta el segundo comando
        string path = resolvePath(cmd.pipeCmd->args[0]);
        char** argv = new char*[cmd.pipeCmd->args.size() + 1];
        for (size_t i = 0; i < cmd.pipeCmd->args.size(); i++) {
            argv[i] = const_cast<char*>(cmd.pipeCmd->args[i].c_str());
        }
        argv[cmd.pipeCmd->args.size()] = nullptr;
        execvp(path.c_str(), argv);
        
        // Si execvp falla
        cerr << "Error: No se pudo ejecutar '" << cmd.pipeCmd->args[0] << "': " << strerror(errno) << endl;
        exit(1);
    }
    
    // --- Codigo del Proceso Padre ---
    // Cierra ambos extremos del pipe, ya no los necesita
    close(pipefd[0]);
    close(pipefd[1]);
    
    // Espera a que ambos hijos terminen
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}

void Executor::execute(const Command& cmd) {
    if (cmd.hasPipe) {
        executePipe(cmd);
    } else {
        executeSimple(cmd);
    }
}
