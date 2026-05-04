/*
 * ============================================================================
 * FILE: igame_state_renderer.h
 * PURPOSE: Abstract strategy for rendering different game states - Strategy Pattern
 *          REFACTORED: Accepts ONLY const references to Model objects
 * 
 * RESPONSIBILITY: Define interface for rendering different game states
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP)
 *   - OPEN for extension: New game states/screens (pause menu, difficulty selector,
 *     achievements, etc.) can be added by creating new IGameStateRenderer implementations
 *   - CLOSED for modification: GameEngine doesn't change when adding new UI screens
 * 
 * STRATEGY PATTERN: Each IGameStateRenderer implementation handles rendering for a
 *   specific game state (playing, menu, game_over, pause, etc.) without cluttering
 *   the main GameEngine with conditional logic.
 * 
 * MVC COMPLIANCE: 
 *   - View layer interface depends on Model objects (Board, Tetromino)
 *   - DOES NOT accept raw SDL_Renderer pointers or game engine references
 *   - Accepts only const references - View reads data but cannot mutate Model
 *   - Renderer provides SDL context internally
 * 
 * VIEW LAYER COMPONENT: Rendering interface for MVC View layer
 * ============================================================================
 */

#ifndef IGAME_STATE_RENDERER_H
#define IGAME_STATE_RENDERER_H

// Forward declarations (no need to include model headers in interface)
class Board;
class Tetromino;

class IGameStateRenderer {
public:
    virtual ~IGameStateRenderer() = default;
    
    /**
     * Render the current game state
     * 
     * PRECONDITIONS:
     *   - board is valid and initialized
     *   - tetromino is valid and initialized
     *   - score is non-negative
     *   - Internal SDL_Renderer is valid and initialized
     * 
     * POSTCONDITIONS:
     *   - Game state rendered to SDL_Renderer
     *   - Display ready for presentation
     *   - No Model objects mutated (const references enforced)
     * 
     * INVARIANT:
     *   - Multiple render calls produce same visual output (for same inputs)
     *   - No side effects beyond rendering
     *   - View does not modify Model
     * 
     * MVC COMPLIANCE:
     *   - Accept const Board& - View reads data, cannot modify
     *   - Accept const Tetromino& - View reads data, cannot modify
     *   - No GameEngine references - View is independent
     *   - No raw pointers - Value semantics via references
     * 
     * @param board Const reference to game board state (read-only)
     * @param tetromino Const reference to current tetromino (read-only)
     * @param score Current player score (value semantics)
     */
    virtual void render(const Board& board, const Tetromino& tetromino, int score) = 0;
    
    /**
     * Get the name of this renderer (for debugging/logging)
     * 
     * PRECONDITIONS:
     *   - Object is properly constructed
     * 
     * POSTCONDITIONS:
     *   - Returns valid C-string (never nullptr)
     *   - String is null-terminated
     *   - Name is consistent across multiple calls
     * 
     * @return Renderer name (e.g., "PlayingRenderer", "MenuRenderer")
     */
    virtual const char* getName() const = 0;
};

#endif // IGAME_STATE_RENDERER_H
