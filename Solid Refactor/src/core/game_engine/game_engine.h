/*
 * ============================================================================
 * FILE: game_engine.h
 * PURPOSE: GameEngine class - Orchestrates the game heartbeat and coordinates components
 *          NOW USING SEGREGATED INTERFACES (ISP Refactoring)
 * 
 * RESPONSIBILITY: Manage game loop and inter-component communication
 * 
 * INTERFACE SEGREGATION PRINCIPLE (ISP) Improvements:
 *   - Now depends on IInputProvider instead of concrete InputHandler
 *   - Uses IGameStateRenderer and IScreenRenderer instead of generic GameRenderer
 *   - Receives exactly the interfaces it needs (nothing more)
 *   - Reduces coupling to specific implementations
 *   - Enables testing with mock interfaces
 * 
 * SINGLE RESPONSIBILITY: This class is responsible ONLY for:
 *   - Managing the game loop (heartbeat)
 *   - Holding instances of Board, Tetromino, Renderer, InputProvider
 *   - Coordinating communication between components
 *   - Managing game state and transitions
 *   - Handling collision detection and piece locking
 *   - Managing score and game over conditions
 *   - Delegating to strategy patterns for extensible behavior
 * 
 * DESIGN IMPROVEMENTS:
 *   - ScoringStrategy: New scoring systems can be added without modifying this class
 *   - IGameStateRenderer: New game renderers can be added without modification
 *   - IScreenRenderer: New screen renderers can be added without modification
 *   - IInputProvider: Multiple input sources can be used
 *   - Main.cpp remains completely unchanged
 * 
 * COORDINATOR PATTERN: This class ties together:
 *   - Board: Manages game grid state
 *   - Tetromino: Manages piece data (now implements ITransformable)
 *   - Renderer: Displays game state
 *   - IInputProvider: Processes user input (interface, not concrete class)
 *   - ScoringStrategy: Calculates scores
 *   - IGameStateRenderer & IScreenRenderer: Render different game states
 * ============================================================================
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include "../board/board.h"
#include "../tetromino/tetromino.h"
#include "../interfaces/scoring_strategy.h"
#include "../interfaces/game_state_renderer.h"
#include "../interfaces/screen_renderer.h"
#include "../interfaces/input_provider.h"
#include "../../ui/renderer/renderer.h"

enum class GameStateEnum {
    MENU,
    PLAYING,
    GAME_OVER,
    QUIT
};

class GameEngine {
private:
    // Game components
    Board board;
    Tetromino tetromino;
    std::unique_ptr<IInputProvider> inputProvider;  // ISP: Depends on interface, not concrete class
    Renderer renderer;
    
    // Strategy patterns (OCP-compliant)
    std::unique_ptr<ScoringStrategy> scoringStrategy;
    std::unique_ptr<IGameStateRenderer> playingRenderer;    // ISP: Segregated interface
    std::unique_ptr<IScreenRenderer> menuRenderer;          // ISP: Segregated interface
    std::unique_ptr<IGameStateRenderer> gameOverRenderer;   // ISP: Segregated interface
    
    // Game state
    GameStateEnum gameState;
    int score;
    int level;  // Added for level-based scoring
    bool gameOver;
    
    // Timing
    int lastDropTime;
    int dropDelay;  // Milliseconds between automatic drops
    
    // Helper methods
    bool checkCollision(int dx, int dy);
    void lockPieceToBoard();
    void spawnNewPiece();
    void handleInput();
    void updateGameLogic(int currentTime);
    
public:
    GameEngine(SDL_Renderer* sdlRenderer, TTF_Font* sdlFont);
    ~GameEngine();
    
    // Game loop
    void update(int currentTime);
    void render();
    
    // Game state queries
    GameStateEnum getGameState() const { return gameState; }
    void startNewGame();
    void setGameState(GameStateEnum state) { gameState = state; }
    int getScore() const { return score; }
    int getLevel() const { return level; }
    bool isGameOver() const { return gameOver; }
    bool shouldQuit() const { return inputProvider ? inputProvider->isQuitRequested() : false; }
    
    // Strategy injection (for customization)
    void setScoringStrategy(std::unique_ptr<ScoringStrategy> strategy) {
        scoringStrategy = std::move(strategy);
    }
    
    // ISP: Input provider injection (allows testing with mock providers)
    void setInputProvider(std::unique_ptr<IInputProvider> provider) {
        inputProvider = std::move(provider);
    }
};

#endif // GAME_ENGINE_H
