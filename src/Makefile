CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g
LDFLAGS = -lreadline

SOURCES = main.cpp Shell.cpp Parser.cpp Executor.cpp Builtin.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = minishell

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "✓ Compilación exitosa!"
	@echo "  Ejecuta con: ./minishell"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "✓ Archivos limpiados"

run: all
	./$(TARGET)

debug: all
	gdb ./$(TARGET)

valgrind: all
	valgrind --leak-check=full ./$(TARGET)

.PHONY: all clean run debug valgrind
