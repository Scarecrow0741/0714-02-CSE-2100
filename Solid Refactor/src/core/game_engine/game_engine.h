/*
 * ============================================================================
 * FILE: game_engine.h
 * PURPOSE: GameEngine class - Orchestrates the game heartbeat and coordinates components
 *          NOW USING STRATEGY PATTERNS FOR SCORING AND RENDERING (OCP-COMPLIANT)
 * 
 * RESPONSIBILITY: Manage game loop and inter-component communication
 * 
 * SINGLE RESPONSIBILITY: This class is responsible ONLY for:
 *   - Managing the game loop (heartbeat)
 *   - Holding instances of Board, Tetromino, Renderer, InputHandler
 *   - Coordinating communication between components
 *   - Managing game state and transitions
 *   - Handling collision detection and piece locking
 *   - Managing score and game over conditions
 *   - Delegating to strategy patterns for extensible behavior
 * 
 * DESIGN IMPROVEMENTS:
 *   - ScoringStrategy: New scoring systems can be added without modifying this class
 *   - GameRenderer: New game states/UI screens can be added without modification
 *   - Main.cpp remains completely unchanged
 * 
 * COORDINATOR PATTERN: This class ties together:
 *   - Board: Manages game grid state
 *   - Tetromino: Manages piece data (now with polymorphic shapes)
 *   - Renderer: Displays game state
 *   - InputHandler: Processes user input
 *   - ScoringStrategy: Calculates scores
 *   - GameRenderer: Renders different game states
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
#include "../interfaces/game_renderer.h"
#include "../../input/input_handler.h"
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
    InputHandler inputHandler;
    Renderer renderer;
    
    // Strategy patterns (OCP-compliant)
    std::unique_ptr<ScoringStrategy> scoringStrategy;
    std::unique_ptr<GameRenderer> playingRenderer;
    std::unique_ptr<GameRenderer> menuRenderer;
    std::unique_ptr<GameRenderer> gameOverRenderer;
    
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
    bool shouldQuit() const { return inputHandler.isQuitRequested(); }
    
    // Strategy injection (for customization)
    void setScoringStrategy(std::unique_ptr<ScoringStrategy> strategy) {
        scoringStrategy = std::move(strategy);
    }
};

#endif // GAME_ENGINE_H
