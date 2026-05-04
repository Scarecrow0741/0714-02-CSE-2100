/*
 * ============================================================================
 * FILE: renderer.h
 * PURPOSE: Renderer class - Handles all game visualization
 *          VIEW LAYER - Moved from ui/renderer/
 * 
 * RESPONSIBILITY: Render game state to screen using SDL
 * 
 * SINGLE RESPONSIBILITY: This class is responsible ONLY for:
 *   - Rendering the Board (placed blocks)
 *   - Rendering the Tetromino (falling piece)
 *   - Drawing UI elements (HUD, score, next piece preview)
 *   - Drawing grid lines and visual guides
 *   - Using polymorphic shapes to determine colors (no hardcoded mappings)
 * 
 * This class DOES NOT:
 *   - Perform game logic
 *   - Handle collision detection
 *   - Manage piece rotation or movement
 *   - Know about menu states or game state machine
 *   - Know about specific shape types (delegates to shapes for colors)
 *   - Mutate Model objects
 * 
 * VIEW LAYER: May include SDL/TTF and read Model data (const references)
 * ============================================================================
 */

#ifndef RENDERER_H
#define RENDERER_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Model layer - read-only access
#include "../model/board.h"
#include "../model/tetromino.h"

#define BLOCK_SIZE 20
#define WINDOW_BORDER 50
#define HUD_WIDTH 200
#define NEXT_PREVIEW_SIZE 4
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

class Renderer {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    int boardX;
    int boardY;
    
    // Helper methods for rendering
    void drawBoard(const Board& board);
    void drawFallingPiece(const Tetromino& tetromino, const Board& board);
    void drawGridLines();
    void drawHUD(int score, const Tetromino& tetromino);
    void drawBackground();
    
    // Color utilities (now polymorphic!)
    void setColorForPieceType(int type);
    
public:
    Renderer(SDL_Renderer* sdlRenderer, TTF_Font* sdlFont);
    ~Renderer();
    
    // Main rendering - accepts const references to Model objects
    void render(const Board& board, const Tetromino& tetromino, int score);
    void renderMenu(int selectedOption);
    void renderGameOver(int finalScore);
    
    // Getters for window dimensions
    int getBoardPixelWidth() const { return BOARD_WIDTH * BLOCK_SIZE; }
    int getBoardPixelHeight() const { return BOARD_HEIGHT * BLOCK_SIZE; }
    
    // Accessor for SDL renderer (needed by View layer for UI rendering)
    SDL_Renderer* getSDLRenderer() const { return renderer; }
    TTF_Font* getFont() const { return font; }
};

#endif // RENDERER_H
