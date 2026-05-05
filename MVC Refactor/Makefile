# Makefile for Tetris MVC Refactored Version
# Compiles and runs the game with new mvc/ directory structure

CXX = g++
CXXFLAGS = -std=c++17
LDFLAGS = -L./lib -lSDL2 -lSDL2_ttf -lm
INCLUDE = -I./mvc/config -I./src/include

# Source files (updated to mvc/ structure)
SOURCES = \
	mvc/model/board/board.cpp \
	mvc/model/tetromino/tetromino.cpp \
	mvc/model/tetromino/tetromino_shapes.cpp \
	mvc/controller/game_engine/game_engine.cpp \
	mvc/controller/input/input_handler.cpp \
	mvc/view/renderer/renderer.cpp \
	mvc/view/menu/button.cpp \
	mvc/view/menu/menu_screen.cpp \
	mvc/view/menu/game_over_screen.cpp \
	mvc/view/menu/menu.cpp \
	mvc/config/main.cpp

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
	rm -f $(TARGET)

# Rebuild (clean + build)
rebuild: clean all

# Phony targets
.PHONY: all run clean rebuild
