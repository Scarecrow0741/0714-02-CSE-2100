/*
 * ============================================================================
 * FILE: renderer.h
 * PURPOSE: Renderer class - Handles all game visualization
 *          NOW SUPPORTING POLYMORPHIC SHAPE COLORS (OCP-COMPLIANT)
 * 
 * RESPONSIBILITY: Render game state to screen
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
 * ============================================================================
 */

#ifndef RENDERER_H
#define RENDERER_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Board;
class Tetromino;

#define BLOCK_SIZE 20
#define WINDOW_BORDER 50
#define HUD_WIDTH 200
#define NEXT_PREVIEW_SIZE 4

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
    
    // Main rendering
    void render(const Board& board, const Tetromino& tetromino, int score);
    void renderMenu(int selectedOption);
    void renderGameOver(int finalScore);
    
    // Getters for window dimensions
    int getBoardPixelWidth() const { return 20 * BLOCK_SIZE; }
    int getBoardPixelHeight() const { return 20 * BLOCK_SIZE; }
    
    // New: Accessor for SDL renderer (needed by GameRenderer strategy)
    SDL_Renderer* getSDLRenderer() const { return renderer; }
};

#endif // RENDERER_H
