# 🐚 Mini-Shell en C++

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
![Linux](https://img.shields.io/badge/OS-Linux-success.svg)
![Status](https://img.shields.io/badge/Estado-En%20Desarrollo-orange)
![License](https://img.shields.io/badge/Licencia-Acad%C3%A9mica-lightgrey)

---

## 📘 Descripción del Proyecto

**Mini-Shell** es un intérprete de comandos desarrollado en **C++** para sistemas **Linux**, como parte del curso de **Sistemas Operativos** en la **Universidad Nacional Jorge Basadre Grohmann (UNJBG)**.  

El objetivo del proyecto es implementar un entorno similar a **Bash**, capaz de ejecutar **comandos externos**, manejar **redirecciones de entrada/salida**, **pipes**, **procesos en segundo plano** y **comandos internos (built-ins)**.

---

## ⚙️ Requerimientos

Para compilar y ejecutar el proyecto, asegúrate de tener instalado en tu entorno Linux:

- 🧩 **G++** (compatible con estándar **C++17** o superior)  
- ⚙️ **GNU Make** (para la automatización de la compilación)  
- 📜 **Biblioteca Readline** (para soporte de historial y edición interactiva en la línea de comandos)

### 📦 Instalación de dependencias

**Debian / Ubuntu:**
```bash
sudo apt-get install libreadline-dev
```

**Fedora / CentOS:**
```bash
sudo dnf install readline-devel
```

---

## 🧱 Compilación y Ejecución

### 🌀 1. Clonar el repositorio
```bash
git clone <URL-DEL-REPOSITORIO>
cd <nombre-del-directorio>
```

### 🛠️ 2. Compilar el proyecto
```bash
make
```

### ▶️ 3. Ejecutar la Mini-Shell
```bash
./minishell
```

### 🧹 4. Limpiar archivos generados
```bash
make clean
```

---

## 🚀 Funcionalidades Implementadas

| 💡 **Funcionalidad Base** | 🧾 **Descripción** |
|----------------------------|--------------------|
| 🖥️ **Prompt Personalizado** | Muestra el nombre del directorio actual en color para mejor visualización. |
| 📂 **Resolución de Rutas** | Soporta rutas absolutas (`/bin/ls`) y busca en `/bin` y `/usr/bin` para rutas relativas. |
| ⚙️ **Ejecución con fork/exec** | Los comandos externos se ejecutan en procesos hijos mediante `fork()` y `execvp()`. |
| ⏳ **Sincronización con waitpid()** | Espera la finalización de procesos en primer plano antes de mostrar el siguiente prompt. |
| ❌ **Manejo de Errores** | Informa claramente al usuario mediante `perror()` o `strerror()`. |
| 📤 **Redirección de Salida (`>`)** | Envía la salida de un comando a un archivo (crea o sobrescribe). |
| 🔚 **Comando de Salida** | Termina la shell con `exit` o `salir`. |

---

## 💎 Extensiones de Valor Agregado

| ✨ **Extensión** | 🧾 **Descripción** |
|------------------|--------------------|
| 🔗 **Pipes (`|`)** | Permite encadenar comandos conectando la salida de uno con la entrada de otro. |
| 🧠 **Ejecución en Segundo Plano (`&`)** | Ejecuta procesos sin bloquear la shell, mostrando el PID del proceso creado. |
| 📥 **Redirección de Entrada (`<`)** | Toma la entrada desde un archivo en lugar del teclado. |
| 📑 **Doble Redirección (`>>`)** | Añade la salida al final de un archivo sin sobrescribirlo. |
| ⚡ **Comandos Internos (Built-ins)** | Incluye `cd`, `pwd`, `help`, `history` y `alias`. |
| 🛡️ **Manejo de Señales (SIGINT)** | Captura `Ctrl + C` para evitar el cierre accidental de la shell. |

---

## 🧪 Casos de Prueba y Ejemplos de Uso

### 📄 Comandos básicos
```bash
ls -l
pwd
cd ..
```

### 🔁 Redirección y pipes
```bash
ls > archivos.txt
cat archivos.txt | grep .cpp
```

### 🧩 Ejecución en segundo plano
```bash
./programa &
```

### ⚙️ Comandos internos
```bash
cd /home/usuario
alias ll='ls -l'
history
help
```

---

## 🧑‍💻 Autores

Desarrollado por estudiantes de **Ingeniería de Sistemas**  
**Universidad Nacional Jorge Basadre Grohmann (UNJBG)**  
📘 Curso: **Sistemas Operativos**





