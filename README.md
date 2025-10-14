# mini-shell
Mini-Shell en C++ para Sistemas Operativos
Este proyecto es una implementación de un intérprete de comandos (mini-shell) para sistemas Linux, desarrollado en C++ como parte de la asignatura de Sistemas Operativos de la UNJBG. El programa es capaz de ejecutar comandos externos, manejar redirecciones de E/S, tuberías (pipes), ejecución en segundo plano y comandos internos (built-ins).

Requerimientos
Para compilar y ejecutar este proyecto, necesitas tener instalado lo siguiente en un entorno Linux:

Compilador G++: Que soporte el estándar C++17.

GNU Make: Para automatizar el proceso de compilación.

Biblioteca Readline: Para una mejor experiencia en la línea de comandos (historial con flechas, etc.).

Para instalar en Debian/Ubuntu: sudo apt-get install libreadline-dev

Para instalar en Fedora/CentOS: sudo dnf install readline-devel

Instrucciones de Compilación y Ejecución
Clonar el repositorio:

Bash

git clone <URL-DE-TU-REPOSITORIO>
cd <nombre-del-directorio>
Compilar el proyecto:
Usa el comando make para compilar todos los archivos fuente. 

Bash

make
Ejecutar la Mini-Shell:
Una vez compilado, puedes iniciar el intérprete con el siguiente comando: 

Bash

./minishell
Limpiar los archivos generados:
Para eliminar los archivos objeto (.o) y el ejecutable, usa:

Bash

make clean

Tabla de Características Implementadas
El proyecto cumple con todas las especificaciones base y añade varias extensiones de valor agregado, superando el mínimo requerido de dos.

Característica	Estado	Descripción
--- Funcionalidades Base ---		
Prompt Personalizado
Muestra el nombre del directorio actual en color para una mejor visualización. 

Resolución de Rutas
Ejecuta comandos con rutas absolutas (/bin/ls) y busca en /bin y /usr/bin para rutas relativas (ls). 

Ejecución con fork/exec
Cada comando externo se ejecuta en un proceso hijo, creado con fork() y reemplazado con execvp(). 

Espera del Padre (waitpid)
La shell espera a que los comandos en primer plano terminen antes de mostrar el siguiente prompt. 

Manejo de Errores
Informa al usuario si un comando no se encuentra o si una llamada al sistema falla, usando perror o strerror. 

Redirección de Salida (>)
Redirige la salida estándar de un comando a un archivo, creándolo o truncándolo si ya existe. 

Comando de Salida
La shell termina de forma segura al ingresar salir o exit. 

--- Extensiones de Valor Agregado ---		
**Pipes (`	`)**
Tareas en Segundo Plano (&)	
Permite ejecutar comandos en background sin bloquear el prompt, mostrando el PID del nuevo proceso. 

Redirección de Entrada (<)
Redirige el contenido de un archivo a la entrada estándar de un comando. 

Doble Red. de Salida (>>)
Redirige la salida estándar a un archivo, pero añadiéndola al final (append) en lugar de sobrescribir. 

Comandos Internos (Built-ins)
Incluye cd, pwd, help, history y alias para gestionar la shell sin crear nuevos procesos. 

Manejo de Señales (SIGINT)
Captura la señal Ctrl+C para evitar que la shell se cierre accidentalmente, mostrando una nueva línea de prompt en su lugar

Casos de Prueba y Ejemplos de Uso
A continuación se muestran algunos ejemplos de cómo interactuar con la mini-shell.

