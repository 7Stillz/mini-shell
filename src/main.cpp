#include <iostream>
#include <string>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "Shell.h"

using namespace std;

void signal_handler(int signo) {
    if (signo == SIGINT) {
        cout << "\n";
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

int main() {
    signal(SIGINT, signal_handler);
    
    Shell shell;
    
    cout << "╔════════════════════════════════════════╗\n";
    cout << "║      Mini-Shell                        ║\n";
    cout << "║      Escribe 'help' para ayuda         ║\n";
    cout << "║      Escribe 'salir' para terminar     ║\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    
    shell.run();
    
    cout << "¡Hasta luego!\n";
    return 0;
}
