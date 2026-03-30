# Makefile for Tetris OOP Refactored Version
# Compiles and runs the game

CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = -lSDL2 -lSDL2_ttf -lm
INCLUDE = -I./src/include

# Source files
SOURCES = \
	src/core/board/board.cpp \
	src/core/tetromino/tetromino.cpp \
	src/core/game_engine/game_engine.cpp \
	src/input/input_handler.cpp \
	src/ui/renderer_new.cpp \
	src/ui/menu.cpp \
	src/main/main.cpp

# Output
TARGET = tetris_oop.exe

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
	rm -f $(TARGET)

# Rebuild (clean + build)
rebuild: clean all

# Phony targets
.PHONY: all run clean rebuild
