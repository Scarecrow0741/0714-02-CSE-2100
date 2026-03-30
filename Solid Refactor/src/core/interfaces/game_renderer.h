/*
 * ============================================================================
 * FILE: game_renderer.h
 * PURPOSE: Abstract strategy for rendering different game states - Strategy Pattern
 * 
 * RESPONSIBILITY: Define interface for rendering different game states
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP)
 *   - OPEN for extension: New game states/screens (pause menu, difficulty selector,
 *     achievements, etc.) can be added by creating new GameRenderer implementations
 *   - CLOSED for modification: GameEngine doesn't change when adding new UI screens
 * 
 * STRATEGY PATTERN: Each GameRenderer implementation handles rendering for a
 *   specific game state (playing, menu, game_over, pause, etc.) without cluttering
 *   the main GameEngine with conditional logic.
 * ============================================================================
 */

#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <SDL2/SDL.h>

class Board;
class Tetromino;

class GameRenderer {
public:
    virtual ~GameRenderer() = default;
    
    /**
     * Render the current game state
     * 
     * @param sdlRenderer SDL renderer to draw to
     * @param board Game board state
     * @param tetromino Current tetromino piece
     * @param score Current player score
     */
    virtual void render(SDL_Renderer* sdlRenderer, const Board& board, 
                       const Tetromino& tetromino, int score) = 0;
    
    /**
     * Get the name of this renderer (for debugging)
     * @return Renderer name (e.g., "PlayingRenderer", "MenuRenderer")
     */
    virtual const char* getName() const = 0;
};

#endif // GAME_RENDERER_H
