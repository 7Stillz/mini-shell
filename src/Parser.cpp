#include "Parser.h"
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

// Constructor: almacena la linea de comando completa para luego analizarla.
Parser::Parser(const string& commandLine) : line(commandLine) {}

Parser::~Parser() {}

// Divide la linea de comando en tokens (palabras) respetando las comillas.
vector<string> Parser::tokenize(const string& str) {
    vector<string> tokens;
    string token;
    bool inQuotes = false;
    
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        
        // Alterna el estado si encuentra comillas (para mantener espacios dentro de ellas)
        if (c == '"' || c == '\'') {
            inQuotes = !inQuotes;
        }
        // Si encuentra un espacio fuera de comillas, termina un token
        else if (c == ' ' && !inQuotes) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
        // Cualquier otro caracter se agrega al token actual
        else {
            token += c;
        }
    }
    
    // Agrega el ultimo token si existe
    if (!token.empty()) {
        tokens.push_back(token);
    }
    
    return tokens;
}

// Analiza una lista de tokens sin pipes (|) y detecta redirecciones, background, etc.
Command Parser::parseSimple(const vector<string>& tokens) {
    Command cmd;
    
    for (size_t i = 0; i < tokens.size(); i++) {
        if (tokens[i] == ">") {
            if (i + 1 < tokens.size()) {
                cmd.outputFile = tokens[i + 1];
                cmd.appendOutput = false;
                i++;
            }
        }
        else if (tokens[i] == ">>") {
            if (i + 1 < tokens.size()) {
                cmd.outputFile = tokens[i + 1];
                cmd.appendOutput = true;
                i++;
            }
        }
        else if (tokens[i] == "<") {
            if (i + 1 < tokens.size()) {
                cmd.inputFile = tokens[i + 1];
                i++;
            }
        }
        else if (tokens[i] == "&") {
            cmd.background = true;
        }
        else {
            cmd.args.push_back(tokens[i]);
        }
    }
    
    return cmd;
}

// Analiza la linea completa y maneja pipe (|) entre comandos.
Command Parser::parse() {
    vector<string> tokens = tokenize(line);
    
    if (tokens.empty()) {
        return Command();
    }
    
    // Busca si hay una pipe (|) en los tokens
    auto pipePos = find(tokens.begin(), tokens.end(), "|");
    
    // Si existe una pipe, divide en dos comandos (antes y despues del "|")
    if (pipePos != tokens.end()) {
        vector<string> leftTokens(tokens.begin(), pipePos);
        vector<string> rightTokens(pipePos + 1, tokens.end());
	        
        // Crea el comando izquierdo y conecta el derecho mediante un puntero
        Command cmd = parseSimple(leftTokens);
        cmd.hasPipe = true;
        
	cmd.pipeCmd = new Command();
	*cmd.pipeCmd = parseSimple(rightTokens);
        
        return cmd;
    }
    // Si no hay pipes, se analiza un solo comando
    else {
        return parseSimple(tokens);
    }
}
