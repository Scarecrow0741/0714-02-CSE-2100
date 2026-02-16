# Tetris Game - Modular Build System
# Refactored for clean architecture and separation of concerns
# 
# This Makefile organizes the build process to:
# - Compile from modular src/ folder structure
# - Output object files mirroring the source tree in build/obj/
# - Link statically against SDL2
# - Place executable in build/bin/ and copy to project root
#
# Usage:
#   make           - Build the executable
#   make clean     - Remove all build artifacts
#   make rebuild   - Clean and rebuild
#   make run       - Build and run
#   make help      - Show this help text

# ========== Configuration ==========
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -Isrc/core/types -Isrc
LDFLAGS = -mwindows -static
LDLIBS = -lSDL2 -lSDL2_ttf -lmingw32 -lm

# Directories
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
DIST_DIR = .

# ========== Source Files ==========
# Core types and data
SOURCES_CORE_TYPES =  src/core/types/tetris_constants.h \
                      src/core/types/tetris_types.h

# Core pieces
SOURCES_PIECES = src/core/pieces/tetris_pieces.c

# Core board
SOURCES_BOARD = src/core/board/board_state.c \
                src/core/board/board_collision.c

# UI SDL rendering
SOURCES_UI_SDL = src/ui/sdl/sdl_renderer.c

# UI Menu
SOURCES_UI_MENU = src/ui/menu/game_menu.c

# Main entry point
SOURCES_MAIN = src/main/tetris_main.c

# All source files
ALL_SOURCES = $(SOURCES_PIECES) $(SOURCES_BOARD) \
              $(SOURCES_UI_SDL) $(SOURCES_UI_MENU) $(SOURCES_MAIN)

# Object files with path mirroring
OBJECTS = $(patsubst src/%.c,$(OBJ_DIR)/src/%.o,$(ALL_SOURCES))

# Executable
EXECUTABLE = $(BIN_DIR)/tetris.exe
DIST_EXECUTABLE = $(DIST_DIR)/tetris.exe

# ========== Build Rules ==========
.PHONY: all clean rebuild run directories help

all: directories $(EXECUTABLE) $(DIST_EXECUTABLE)

directories:
	@if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	@if not exist "$(OBJ_DIR)\src\core\types" mkdir "$(OBJ_DIR)\src\core\types"
	@if not exist "$(OBJ_DIR)\src\core\pieces" mkdir "$(OBJ_DIR)\src\core\pieces"
	@if not exist "$(OBJ_DIR)\src\core\board" mkdir "$(OBJ_DIR)\src\core\board"
	@if not exist "$(OBJ_DIR)\src\ui\sdl" mkdir "$(OBJ_DIR)\src\ui\sdl"
	@if not exist "$(OBJ_DIR)\src\ui\menu" mkdir "$(OBJ_DIR)\src\ui\menu"
	@if not exist "$(OBJ_DIR)\src\main" mkdir "$(OBJ_DIR)\src\main"
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

$(OBJ_DIR)/src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	@echo.
	@echo Build successful! Executable created at: $(EXECUTABLE)

$(DIST_EXECUTABLE): $(EXECUTABLE)
	copy "$(EXECUTABLE)" "$(DIST_EXECUTABLE)" /Y > nul
	@echo Executable copied to project root: $(DIST_EXECUTABLE)

clean:
	@echo Cleaning build artifacts...
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)" 2 >nul || true
	@if exist "$(DIST_EXECUTABLE)" del "$(DIST_EXECUTABLE)"
	@echo Clean complete.

rebuild: clean all

run: all
	@echo.
	@echo Starting Tetris...
	@echo.
	@$(DIST_EXECUTABLE)

help:
	@echo Tetris Game Build System
	@echo ========================
	@echo.
	@echo Targets (Windows/MinGW):
	@echo   make              - Build the executable
	@echo   make clean        - Remove build artifacts
	@echo   make rebuild      - Clean and rebuild
	@echo   make run          - Build and run the game
	@echo   make directories  - Create build directories
	@echo   make help         - Show this help text
	@echo.
	@echo Build outputs:
	@echo   Object files: build\obj\ (mirrors src\ structure)
	@echo   Executable:   build\bin\tetris.exe
	@echo   Dist copy:    tetris.exe (in project root)
	@echo.
	@echo Configuration:
	@echo   Compiler: $(CC)
	@echo   Flags:    $(CFLAGS)
	@echo   Linking:  $(LDFLAGS) $(LDLIBS)
	@echo.
	@echo For more information, see scripts/REFACTORING_GUIDE.md
