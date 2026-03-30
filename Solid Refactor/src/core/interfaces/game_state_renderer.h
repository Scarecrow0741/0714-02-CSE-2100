/*
 * ============================================================================
 * FILE: game_state_renderer.h
 * PURPOSE: IGameStateRenderer interface - Minimal interface for rendering game state
 *          Part of Interface Segregation Principle (ISP) refactoring
 * 
 * RESPONSIBILITY: Define interface for rendering active game state with board/pieces
 * 
 * DESIGN PRINCIPLE: Interface Segregation Principle (ISP)
 *   - FOCUSED: Only defines game state rendering (playing, game-over)
 *   - SEGREGATED: Separate from IScreenRenderer (menus, UI)
 *   - SEGREGATED: Only requires Board and Tetromino references
 *   - MINIMAL: Clients only depend on specific rendering operations needed
 * 
 * BACKGROUND:
 *   - Previous GameRenderer forced all renderers to accept Board, Tetromino, score
 *   - MenuRenderer doesn't need Board or Tetromino parameters
 *   - By segregating, each renderer only gets what it needs
 *   - Reduces coupling and increases clarity of dependencies
 * 
 * USAGE:
 *   - GameEngine will use IGameStateRenderer for playing/game-over screens
 *   - Renderers for Board-based rendering implement this interface
 *   - Menu renderers implement IScreenRenderer instead
 * ============================================================================
 */

#ifndef GAME_STATE_RENDERER_H
#define GAME_STATE_RENDERER_H

#include <SDL2/SDL.h>

// Forward declarations to avoid circular includes
class Board;
class Tetromino;

/**
 * Interface for rendering active game states
 * 
 * ISP COMPLIANCE:
 *   - This interface is focused on GAME STATE RENDERING only
 *   - Only requires references to Board and Tetromino
 *   - Menu renderers don't implement this (use IScreenRenderer instead)
 *   - Clients only depend on the specific data they need
 * 
 * RATIONALE:
 *   - Segregates game rendering from UI/menu rendering
 *   - PlayingRenderer and GameOverRenderer both render Board + Tetromino
 *   - MenuRenderer doesn't need Board/Tetromino and shouldn't depend on them
 */
class IGameStateRenderer {
public:
    virtual ~IGameStateRenderer() = default;
    
    /**
     * Render the current game state
     * 
     * PRECONDITIONS:
     *   - sdlRenderer is valid and initialized
     *   - board is in valid state
     *   - tetromino is in valid state
     *   - score >= 0
     * 
     * POSTCONDITIONS:
     *   - Game state rendered to sdlRenderer
     *   - Board grid and pieces displayed
     *   - Current tetromino piece displayed at current position
     *   - Score displayed on screen
     *   - No internal state modified
     * 
     * INVARIANT:
     *   - Same inputs produce same rendering
     *   - Multiple calls with same inputs produce identical output
     * 
     * @param sdlRenderer SDL renderer to draw to
     * @param board Game board containing placed pieces
     * @param tetromino Current falling piece
     * @param score Current player score
     */
    virtual void render(SDL_Renderer* sdlRenderer, const Board& board, 
                       const Tetromino& tetromino, int score) = 0;
    
    /**
     * Get human-readable name of this renderer
     * 
     * PRECONDITIONS:
     *   - Object is properly initialized
     * 
     * POSTCONDITIONS:
     *   - Returns valid C-string (never nullptr)
     *   - String describes renderer (e.g., "PlayingRenderer")
     *   - Used for debugging/logging only
     * 
     * @return Renderer name C-string
     */
    virtual const char* getName() const = 0;
};

#endif // GAME_STATE_RENDERER_H
