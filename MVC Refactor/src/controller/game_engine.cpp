/*
 * ============================================================================
 * FILE: game_engine.cpp
 * PURPOSE: Implementation of GameEngine class
 *
 * DEPENDENCY INVERSION PRINCIPLE (DIP):
 *   - No longer creates InputHandler, Renderer, or ScoringStrategy internally
 *   - Receives all dependencies via constructor parameters
 *   - Depends only on abstractions (IInputProvider, ScoringStrategy, IGameStateRenderer)
 *   - Concrete implementations are created and wired in main.cpp (composition root)
 * 
 * CONTROLLER LAYER: Orchestrates Model-View-Input interactions
 *
 * BENEFITS:
 *   - Easy to test: Mock objects can be injected
 *   - Easy to extend: New input/scoring/rendering without changing GameEngine
 *   - Clear dependencies: Constructor shows exactly what GameEngine needs
 *   - Single responsibility: GameEngine orchestrates, doesn't create dependencies
 *
 * ============================================================================
 */

#include "game_engine.h"
#include <SDL2/SDL.h>

GameEngine::GameEngine(
    IInputProvider* inputProvider,
    std::vector<std::unique_ptr<IGameStateRenderer>> renderers,
    std::unique_ptr<ScoringStrategy> scoringStrategy
)
    : board(), tetromino(),
      inputProvider(inputProvider),
      renderers(std::move(renderers)),
      scoringStrategy(std::move(scoringStrategy)),
      gameState(GameStateEnum::MENU), score(0), level(1), gameOver(false),
      lastDropTime(0), dropDelay(DROP_DELAY) {
    
    // All components are now initialized by caller (main.cpp)
    // GameEngine just uses what it's given
}

GameEngine::~GameEngine() {
    // Model components destroyed automatically
    // Renderer pointers destroyed by unique_ptr
}

void GameEngine::startNewGame() {
    board.clear();
    score = 0;
    level = 1;
    gameOver = false;
    tetromino.initialize();
    lastDropTime = SDL_GetTicks();
    gameState = GameStateEnum::PLAYING;
}

bool GameEngine::checkCollision(int dx, int dy) {
    // LSP COMPLIANCE: Collision detection is identical for all shapes
    // No shape gets special collision handling or preconditions
    // All shapes use the same bounding box check against board boundaries and placed blocks
    
    // Save current position
    int originalX = tetromino.getPosX();
    int originalY = tetromino.getPosY();
    
    // Apply the movement
    tetromino.moveBy(dx, dy);
    
    // Check if the piece collides with board borders or placed blocks
    bool collision = false;
    
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (tetromino.getPieceAt(x, y)) {
                int boardX = tetromino.getPosX() + x;
                int boardY = tetromino.getPosY() + y;
                
                // Check boundary conditions (identical for all shapes)
                if (boardX < 0 || boardX >= BOARD_WIDTH ||
                    boardY >= BOARD_HEIGHT) {
                    collision = true;
                    break;
                }
                
                // Check collision with placed blocks (identical for all shapes)
                if (boardY >= 0 && board.isCellOccupied(boardX, boardY)) {
                    collision = true;
                    break;
                }
            }
        }
        if (collision) break;
    }
    
    // Revert to original position if collision detected
    if (collision) {
        tetromino.setPosition(originalX, originalY);
    }
    
    return collision;
}

void GameEngine::lockPieceToBoard() {
    // Retrieve the piece data and lock it to the board
    const int* currentPieceData = tetromino.getCurrentPiece();
    int piece[4][4];
    
    // Convert flat array to 2D array
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            piece[y][x] = currentPieceData[y * 4 + x];
        }
    }
    
    board.lockPieceToBoard(piece, tetromino.getPosX(), tetromino.getPosY(), 
                           tetromino.getCurrentType());
}

void GameEngine::spawnNewPiece() {
    tetromino.spawnNewPiece();
    tetromino.setPosition(8, 0);  // Reset to top-center
    
    // Check if the new piece immediately collides (game over)
    if (checkCollision(0, 0)) {
        gameOver = true;
        gameState = GameStateEnum::GAME_OVER;
    }
}

void GameEngine::handleInput() {
    // Poll events (handles quit events)
    if (inputProvider) {
        inputProvider->pollEvents();
    }
    
    if (gameState == GameStateEnum::MENU) {
        // In menu state, don't process game commands
        return;
    }
    
    if (gameState != GameStateEnum::PLAYING) {
        return;
    }
    
    // Get the current command
    GameCommand cmd = inputProvider ? inputProvider->getCommand() : GameCommand::NONE;
    
    switch (cmd) {
        case GameCommand::MOVE_LEFT:
            checkCollision(-1, 0);
            break;
        case GameCommand::MOVE_RIGHT:
            checkCollision(1, 0);
            break;
        case GameCommand::MOVE_DOWN:
            if (!checkCollision(0, 1)) {
                tetromino.moveBy(0, 1);
            }
            break;
        case GameCommand::ROTATE:
            tetromino.rotate();
            if (checkCollision(0, 0)) {
                // Rotation caused collision, revert
                for (int i = 0; i < 3; i++) {
                    tetromino.rotate();
                }
            }
            break;
        case GameCommand::HARD_DROP:
            while (!checkCollision(0, 1)) {
                tetromino.moveBy(0, 1);
            }
            lastDropTime = SDL_GetTicks() - dropDelay;  // Force lock immediately
            break;
        case GameCommand::QUIT:
            inputProvider->quit();
            break;
        default:
            break;
    }
}

void GameEngine::updateGameLogic(int currentTime) {
    // Auto-drop piece at regular intervals
    if (currentTime - lastDropTime >= dropDelay) {
        if (!checkCollision(0, 1)) {
            tetromino.moveBy(0, 1);
        } else {
            // Piece has reached bottom, lock it
            lockPieceToBoard();
            
            // Clear complete lines
            int linesCleared = board.clearCompleteLines();
            if (linesCleared > 0) {
                score += scoringStrategy->calculateScore(linesCleared, level, score);
            }
            
            // Spawn next piece
            spawnNewPiece();
        }
        lastDropTime = currentTime;
    }
}

void GameEngine::update(int currentTime) {
    // Handle input first (may transition game state)
    handleInput();
    
    // Check quit request
    if (inputProvider && inputProvider->isQuitRequested()) {
        gameState = GameStateEnum::QUIT;
        return;
    }
    
    // Update game logic based on state
    if (gameState == GameStateEnum::MENU) {
        // Menu handling (no game logic)
        return;
    }
    
    if (gameState == GameStateEnum::PLAYING) {
        updateGameLogic(currentTime);
    }
    
    if (gameState == GameStateEnum::GAME_OVER) {
        // Game over state - wait for input to return to menu
        // Input handling will set this
        return;
    }
}

void GameEngine::render() {
    // Select appropriate renderer based on game state
    if (renderers.empty()) {
        return;
    }
    
    // Pass CONST REFERENCES to the renderer
    // View layer reads Model but cannot mutate it
    if (gameState == GameStateEnum::PLAYING && renderers.size() > 0) {
        renderers[0]->render(board, tetromino, score);
    } else if (gameState == GameStateEnum::GAME_OVER && renderers.size() > 1) {
        renderers[1]->render(board, tetromino, score);
    } else if (gameState == GameStateEnum::MENU && renderers.size() > 2) {
        renderers[2]->render(board, tetromino, score);
    }
}

bool GameEngine::shouldQuit() const {
    return gameState == GameStateEnum::QUIT || (inputProvider && inputProvider->isQuitRequested());
}
