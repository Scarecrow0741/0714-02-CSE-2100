# Makefile for Tetris MVC Refactored Version
# Compiles and runs the game

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -L./lib -lSDL2 -lSDL2_ttf -lm
INCLUDE = -I./src

# Source files (MVC structure)
SOURCES = \
	src/model/board.cpp \
	src/model/tetromino.cpp \
	src/model/tetromino_shapes.cpp \
	src/view/renderer.cpp \
	src/view/button.cpp \
	src/view/menu_screen.cpp \
	src/view/game_over_screen.cpp \
	src/controller/input_handler.cpp \
	src/controller/game_engine.cpp \
	main.cpp

# Output
TARGET = tetris.exe

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(TARGET) $(SOURCES) $(LDFLAGS)
	@echo Build Complete: $(TARGET)

# Run the game
run: $(TARGET)
	@echo Starting Tetris...
	@./$(TARGET)

# Clean build artifacts
clean:
	rm -f $(TARGET) *.o

# Rebuild (clean + build)
rebuild: clean all

# Phony targets
.PHONY: all run clean rebuild
