# Tetris Game Features

## Overview
This is a classic Tetris game implementation built with SDL2 and C. The game features a complete gameplay experience with menus, scoring, and multiple game states.

## Core Gameplay Features

### 1. **Multiple Game States**
- **Menu Screen**: Interactive start menu with "Play" and "Exit" options
- **Playing State**: Main gameplay where the player controls falling tetrominos
- **Game Over Screen**: Displays final score with option to return to menu

### 2. **Seven Tetromino Pieces**
The game includes all classic Tetris pieces with color-coding:
- **I-Piece** (Cyan): 4 blocks in a line
- **J-Piece** (Blue): L-shaped piece
- **L-Piece** (Orange): Reverse L-shaped piece
- **O-Piece** (Yellow): 2x2 square block
- **S-Piece** (Green): S-shaped piece
- **T-Piece** (Purple): T-shaped piece
- **Z-Piece** (Red): Z-shaped piece

### 3. **Game Board**
- 20x20 grid-based playing field
- Dark green background for clear visibility
- Grid line display for better block alignment
- Border and padding around the board

## Control Features

### 4. **Keyboard Controls**
- **A / LEFT ARROW**: Move piece left
- **D / RIGHT ARROW**: Move piece right
- **S / DOWN ARROW**: Move piece down (soft drop)
- **W / UP ARROW**: Rotate piece
- **SPACE**: Hard drop (instantly place piece)
- **Q**: Quit game

### 5. **Mouse Controls**
- Click on menu buttons to navigate
- Hover effects on menu buttons
- Interactive button highlighting

## Gameplay Mechanics

### 6. **Piece Movement & Rotation**
- Smooth left/right movement with boundary checking
- Clockwise piece rotation with collision detection
- Wall kick mechanics (piece nudges slightly when rotated near edges)
- Gravity-based automatic piece descent

### 7. **Collision Detection**
- Prevents pieces from going through walls or existing blocks
- Detects when pieces land and locks them in place
- Game over detection when new pieces spawn at occupied positions

### 8. **Line Clearing System**
- Automatically detects completed horizontal lines
- Removes full lines and shifts blocks above downward
- Score increases by (lines_cleared)² × 100
- Multiple line clears award exponentially more points

### 9. **Score System**
- Points awarded based on lines cleared
- Single line: 100 points
- Two lines: 400 points
- Three lines: 900 points
- Four lines (Tetris): 1600 points
- Real-time score display during gameplay

## Visual Features

### 10. **Next Piece Preview**
- Shows the next tetromino that will spawn
- Displays in 4x4 preview box on the HUD
- Updated each time a piece is placed
- Helps players plan strategy

### 11. **Color-Coded Blocks**
- Each tetromino type has a distinct color
- Easy to distinguish between piece types
- Enhances visual appeal and gameplay

### 12. **HUD (Heads-Up Display)**
- Located on the right side of the game board
- Shows current score in real-time
- Displays next piece preview
- Updates continuously during gameplay

### 13. **Enhanced Menu Interface**
- Stylized buttons with shadow effects
- Hover effects showing button interactivity
- Grid pattern background
- Large, readable "TETRIS" title with bright green color
- Golden yellow highlighting for selected menu option

### 14. **Game Over Screen**
- Red-themed display indicating game end
- Displays final score prominently
- Option to press ENTER to return to menu
- Grid pattern background for consistency

## Technical Features

### 15. **SDL2 Graphics Rendering**
- Hardware-accelerated rendering with vertical sync (VSync)
- Smooth 60 FPS gameplay
- Dynamic window sizing
- Texture-based text rendering

### 16. **Font Support**
- TTF font integration for text rendering
- Uses Arial font at 24pt size
- Solid text rendering for menu and score display
- Support for colored text

### 17. **Timing & Game Loop**
- 500ms drop interval (pieces fall every 0.5 seconds)
- Frame rate capped at ~125 FPS (8ms per frame)
- Delta time calculation for consistent gameplay
- Drop accumulation system for smooth piece descent

### 18. **Random Piece Generation**
- Random tetromino selection for current and next piece
- Seeded by system time for variety
- Independent randomization for each new piece

## Summary
This Tetris implementation provides a complete, functional game experience with classic mechanics, colorful visuals, interactive menus, and smooth gameplay. It combines essential Tetris features with an intuitive interface and responsive controls.
