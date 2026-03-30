/*
 * ============================================================================
 * FILE: game_engine.h
 * PURPOSE: GameEngine class - Orchestrates the game heartbeat and coordinates components
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
 * 
 * COORDINATOR PATTERN: This class ties together:
 *   - Board: Manages game grid state
 *   - Tetromino: Manages piece data
 *   - Renderer: Displays game state
 *   - InputHandler: Processes user input
 * ============================================================================
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../board/board.h"
#include "../tetromino/tetromino.h"
#include "../../input/input_handler.h"
#include "../../ui/renderer.h"

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
    
    // Game state
    GameStateEnum gameState;
    int score;
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
    bool isGameOver() const { return gameOver; }
    bool shouldQuit() const { return inputHandler.isQuitRequested(); }
};

#endif // GAME_ENGINE_H
