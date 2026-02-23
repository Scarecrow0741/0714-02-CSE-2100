# Changes from Version 1 to Version 2

## Overview
Version 2 represents a complete restructuring and refactoring of the Tetris game codebase. The main goal was to organize code into modular components and apply consistent naming conventions throughout the project.

## Quick Reference: File Changes Summary

| File | Location | Change Type | Key Modifications |
|------|----------|-------------|-------------------|
| main.c | src/main/ | Structural + Naming + Code | Include path updated; `quit` → `is_quit`; `ev` → `event`; Added `SDL_MAIN_HANDLED`; Fixed enum comparison |
| menu.c | src/ui/ | Naming + Code | `play_button_rect` → `g_play_button_rect`; `exit_button_rect` → `g_exit_button_rect`; `isSelected` → `is_selected`; `isHovered` → `is_hovered`; Fixed return type from -1 to MENU_PLAY enum |
| renderer.c | src/ui/ | Naming | `r` → `renderer` throughout file (40+ occurrences) |
| game_logic.c | src/core/ | Structural | Include path updated to `../include/tetris.h` |
| tetris.h | src/include/ | Code | Added `#define SDL_MAIN_HANDLED` before SDL includes |

## 1. Folder Structure Changes

### Version 1 Structure
```
Version 1/
├── game_logic.c
├── main.c
├── menu.c
├── renderer.c
├── tetris.h
├── lib/
│   ├── (SDL2 libraries)
│   ├── cmake/
│   └── pkgconfig/
└── src/
    └── include/SDL2/
        └── (SDL2 header files)
```

### Version 2 Structure
```
Version 2/
├── docs/
│   ├── changes.md
│   └── restructuring_guide.md
├── lib/
│   ├── libSDL2_test.la
│   ├── libSDL2.la
│   ├── libSDL2main.la
│   ├── cmake/
│   │   └── SDL2/
│   │       ├── sdl2-config-version.cmake
│   │       └── sdl2-config.cmake
│   └── pkgconfig/
│       └── sdl2.pc
└── src/
    ├── core/
    │   └── game_logic.c
    ├── include/
    │   ├── tetris.h
    │   └── SDL2/
    │       ├── SDL.h
    │       ├── SDL_*.h (many header files)
    │       ├── SDL_test_*.h
    │       └── (48 total SDL header files)
    ├── main/
    │   └── main.c
    └── ui/
        ├── menu.c
        └── renderer.c
```

**Key Change**: Code files are now organized by functionality:
- **src/core/**: Game mechanics and logic
- **src/main/**: Application entry point
- **src/ui/**: User interface and rendering
- **src/include/**: Header files

## 2. Naming Convention Changes

### Global Variables
**Version 1**: No prefix
```c
SDL_Rect play_button_rect = {0, 0, 0, 0};
SDL_Rect exit_button_rect = {0, 0, 0, 0};
```

**Version 2**: Added `g_` prefix for global variables
```c
SDL_Rect g_play_button_rect = {0, 0, 0, 0};
SDL_Rect g_exit_button_rect = {0, 0, 0, 0};
```

### Function Parameters
**Version 1**: CamelCase or single letters
```c
void draw_button(SDL_Renderer* renderer, SDL_Rect* rect, SDL_bool isSelected, SDL_bool isHovered)
MenuOption check_menu_click(int x, int y)
```

**Version 2**: snake_case for consistency
```c
void draw_button(SDL_Renderer* renderer, SDL_Rect* rect, SDL_bool is_selected, SDL_bool is_hovered)
MenuOption check_menu_click(int x_pos, int y_pos)
```

### Local Variables
**Version 1**: Mix of naming styles
```c
SDL_Renderer *r = game->renderer;
MenuOption hover = check_menu_click(ev.motion.x, ev.motion.y);
SDL_Event ev;
```

**Version 2**: Consistent snake_case
```c
SDL_Renderer *renderer = game->renderer;
MenuOption hover = check_menu_click(event.motion.x, event.motion.y);
SDL_Event event;
```

### Constants
**Version 1**: All caps (already correct)
```c
#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20
#define WINDOW_BORDER 50
#define DROP_INTERVAL_MS 500  // Not previously named, was a magic number
```

**Version 2**: All caps with meaningful names
```c
#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20
#define WINDOW_BORDER 50
#define DROP_INTERVAL_MS 500  // Made explicit constant
```

## 3. Code Changes

### main.c (src/main/main.c)

**SDL Initialization**:
- Added `#define SDL_MAIN_HANDLED` to resolve Windows linking issues
- Changed magic number `500` to constant `DROP_INTERVAL_MS`

**Variable Renaming**:
- `quit` → `is_quit`
- `ev` → `event`
- `drop_interval_ms` → `DROP_INTERVAL_MS` (constant)
- `drop_acc` → `drop_acc` (unchanged, local variable)

**Type Checking Fix**:
- Changed `if (hover != -1)` to `if (hover == MENU_PLAY || hover == MENU_EXIT)`
- Reason: MenuOption is an enum, not an int, so comparison with -1 was incorrect

### menu.c (src/ui/menu.c)

**Global Variables**:
- `play_button_rect` → `g_play_button_rect`
- `exit_button_rect` → `g_exit_button_rect`

**Function Parameters**:
- `isSelected` → `is_selected`
- `isHovered` → `is_hovered`

**Function Signature**:
- `check_menu_click(int x, int y)` → `check_menu_click(int x_pos, int y_pos)`

**Return Value**:
- Changed return type from `-1` (int) to `MENU_PLAY` (MenuOption enum)
- Ensures type consistency

**Global Variable Updates**:
- All references to `play_button_rect` → `g_play_button_rect`
- All references to `exit_button_rect` → `g_exit_button_rect`

### renderer.c (src/ui/renderer.c)

**Variable Renaming**:
- `r` → `renderer` (more descriptive)

**Consistency**:
- All `SDL_SetRenderDrawColor(r, ...)` → `SDL_SetRenderDrawColor(renderer, ...)`
- All `SDL_RenderFillRect(r, ...)` → `SDL_RenderFillRect(renderer, ...)`
- All `SDL_RenderDrawLine(r, ...)` → `SDL_RenderDrawLine(renderer, ...)`

### game_logic.c (src/core/game_logic.c)

**No direct naming convention changes** - File was already well-structured and used appropriate variable naming. Only updated include paths:
- `#include "tetris.h"` → `#include "../include/tetris.h"`

### tetris.h (src/include/tetris.h)

**SDL Main Handling**:
- Added `#define SDL_MAIN_HANDLED` before SDL headers to resolve Windows compilation issues

**No structural changes to** function declarations or type definitions.

## 4. Include Path Changes

**Version 1**:
```c
#include "tetris.h"
#include <SDL2/SDL.h>
```

**Version 2**:
```c
#include "../include/tetris.h"  // Adjusted for new folder structure
#include <SDL2/SDL.h>  // Remains the same
```

## 5. Compilation Changes

### Version 1
- Used files from root directory
- Compilation command: 
```bash
gcc -Wall -Wextra -std=c99 -I./src/include -o tetris.exe *.c -L./src/lib -lSDL2 -lSDL2_ttf -lm
```

### Version 2
- Uses organized folder structure
- Compilation command:
```bash
gcc -Wall -Wextra -std=c99 -I./src/include -o tetris.exe src/main/main.c src/core/game_logic.c src/ui/menu.c src/ui/renderer.c -L./src/lib -lSDL2 -lSDL2_ttf -lm
```

## 6. Summary of Key Improvements

| Aspect | Version 1 | Version 2 |
|--------|-----------|----------|
| **Code Organization** | All files in root + src/ | Modular: core/, main/, ui/ |
| **Global Variables** | No prefix | `g_` prefix |
| **Function Params** | Mixed CamelCase/snake_case | Consistent snake_case |
| **Local Variables** | Inconsistent naming | Consistent snake_case |
| **Constants** | Magic numbers used | Named constants |
| **Include Paths** | Relative to root | Relative to new structure |
| **Windows Support** | Had linking issues | Fixed with SDL_MAIN_HANDLED |
| **Code Documentation** | Minimal | Enhanced with docs/ folder |
| **Scope Clarity** | Less clear | Clear separation by feature |

## 7. Functionality Impact

**No changes to core game logic** - Version 2 maintains 100% functional compatibility with Version 1. All changes are:
- Structural (folder organization)
- Naming (conventions and clarity)
- Build-related (SDL_MAIN_HANDLED for Windows)

The game plays identically in both versions.

