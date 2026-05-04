/*
 * ============================================================================
 * FILE: game_engine.h
 * PURPOSE: GameEngine class - Orchestrates the game heartbeat and coordinates components
 *          CONTROLLER LAYER - Refactored for MVC architecture
 *          NOW USING DEPENDENCY INJECTION (DIP Refactoring)
 * 
 * RESPONSIBILITY: Manage game loop and inter-component communication
 * 
 * DEPENDENCY INVERSION PRINCIPLE (DIP) Improvements:
 *   - GameEngine now depends on IInputProvider interface (not concrete InputHandler)
 *   - GameEngine depends on IGameStateRenderer interface (not concrete Renderer)
 *   - All dependencies injected via constructor
 *   - Composition root (main.cpp) creates and wires all components
 * 
 * SINGLE RESPONSIBILITY: This class is responsible ONLY for:
 *   - Managing the game loop (update and render cycle)
 *   - Holding instances of Board, Tetromino, ScoringStrategy (the Model)
 *   - Reading input via IInputProvider
 *   - Mutating Model objects based on input
 *   - Rendering via IGameStateRenderer with const Model references
 *   - Managing game state transitions
 *   - Handling collision detection and piece locking
 *   - Managing score and game over conditions
 * 
 * This class DOES NOT:
 *   - Create InputHandler, Renderer, or ScoringStrategy (all injected)
 *   - Include SDL headers directly (delegates to View layer)
 *   - Know about specific renderer implementations
 *   - Know about specific input source implementations
 * 
 * ARCHITECTURE: CONTROLLER LAYER
 *   - Owns Model (Board, Tetromino, ScoringStrategy)
 *   - Depends on View (IGameStateRenderer) via interface
 *   - Depends on Input (IInputProvider) via interface
 *   - Coordinates Model updates based on input
 *   - Delegates rendering to View
 *
 * MODEL-VIEW-CONTROLLER FLOW:
 *   Model (Board, Tetromino) ← Controller (GameEngine) ↔ View (IGameStateRenderer)
 *   Input (IInputProvider) → Controller
 * 
 * ============================================================================
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <memory>
#include <vector>

// Model layer includes
#include "../model/board.h"
#include "../model/tetromino.h"
#include "../model/interfaces/scoring_strategy.h"
#include "../model/game_state.h"

// View layer includes (interfaces only)
#include "../../view/interfaces/igame_state_renderer.h"

// Controller layer includes
#include "interfaces/iinput_provider.h"

#define DROP_DELAY 500  // Milliseconds between automatic piece drops
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

/**
 * GameEngine class - Orchestrates the MVC game architecture
 * 
 * DEPENDENCY INJECTION:
 *   - All dependencies provided via constructor
 *   - No concrete dependencies known at compile time
 *   - Easy to test with mock objects
 *   - Supports multiple renderer/input implementations
 * 
 * MVC ARCHITECTURE:
 *   - Controller: GameEngine (this class)
 *   - Model: Board, Tetromino, ScoringStrategy
 *   - View: IGameStateRenderer implementations
 *   - Input: IInputProvider implementations
 */
class GameEngine {
private:
    // ========== MODEL LAYER ==========
    // GameEngine OWNS these model objects
    Board board;
    Tetromino tetromino;
    std::unique_ptr<ScoringStrategy> scoringStrategy;
    
    // ========== VIEW LAYER ==========
    // GameEngine depends on these view interfaces
    std::vector<std::unique_ptr<IGameStateRenderer>> renderers;
    
    // ========== INPUT LAYER ==========
    // GameEngine depends on this input interface
    IInputProvider* inputProvider;  // Not owned (passed in, caller owns lifetime)
    
    // ========== GAME STATE ==========
    GameStateEnum gameState;
    int score;
    int level;
    bool gameOver;
    
    // ========== TIMING ==========
    int lastDropTime;
    int dropDelay;
    
    // ========== HELPER METHODS ==========
    bool checkCollision(int dx, int dy);
    void lockPieceToBoard();
    void spawnNewPiece();
    void handleInput();
    void updateGameLogic(int currentTime);
    
public:
    /**
     * Constructor - Dependency Injection Pattern
     * 
     * PARAMETERS:
     *   - inputProvider: Non-owning pointer to input handler
     *   - renderers: Vector of game state renderers (owned by GameEngine)
     *   - scoringStrategy: Scoring implementation (owned by GameEngine)
     * 
     * POSTCONDITIONS:
     *   - All dependencies stored
     *   - Game initialized but not running
     *   - Game state set to MENU
     * 
     * DIP COMPLIANCE:
     *   - Only accepts interfaces, not concrete types
     *   - Caller (main.cpp) is responsible for object creation
     */
    GameEngine(
        IInputProvider* inputProvider,
        std::vector<std::unique_ptr<IGameStateRenderer>> renderers,
        std::unique_ptr<ScoringStrategy> scoringStrategy
    );
    
    ~GameEngine();
    
    /**
     * Start a new game
     * 
     * POSTCONDITIONS:
     *   - Board cleared
     *   - Tetromino initialized with new piece
     *   - Score reset to 0
     *   - Level reset to 1
     *   - Game state set to PLAYING
     */
    void startNewGame();
    
    /**
     * Update game state for one frame
     * 
     * RESPONSIBILITY:
     *   - Read input via IInputProvider
     *   - Update Model (Board, Tetromino)
     *   - Update score and game state
     *   - NO rendering (delegated to render())
     * 
     * PARAMETERS:
     *   - currentTime: Current tick count from SDL_GetTicks()
     * 
     * POSTCONDITIONS:
     *   - Model updated based on input and game logic
     *   - Game state may transition (PLAYING → GAME_OVER, etc.)
     */
    void update(int currentTime);
    
    /**
     * Render current game state
     * 
     * RESPONSIBILITY:
     *   - Select appropriate IGameStateRenderer based on game state
     *   - Call renderer->render(board, tetromino, score)
     *   - Pass CONST REFERENCES only (View cannot mutate Model)
     *   - NO drawing calls here (delegated to renderers)
     * 
     * POSTCONDITIONS:
     *   - Current game state rendered via IGameStateRenderer
     *   - Screen updated with game visuals
     */
    void render();
    
    /**
     * Check if game should exit
     * 
     * @return true if quit requested, false otherwise
     */
    bool shouldQuit() const;
    
    /**
     * Query current game state
     * @return Current GameStateEnum value
     */
    GameStateEnum getGameState() const { return gameState; }
    
    /**
     * Query current score
     * @return Current player score
     */
    int getScore() const { return score; }
    
    /**
     * Query current level
     * @return Current game level
     */
    int getLevel() const { return level; }
};

#endif // GAME_ENGINE_H
