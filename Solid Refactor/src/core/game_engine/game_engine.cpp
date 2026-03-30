/*
 * ============================================================================
 * FILE: game_engine.cpp
 * PURPOSE: Implementation of GameEngine class
 * ============================================================================
 */

#include "game_engine.h"

#define DROP_DELAY 500  // Milliseconds between automatic piece drops

GameEngine::GameEngine(SDL_Renderer* sdlRenderer, TTF_Font* sdlFont)
    : tetromino(), inputHandler(), renderer(sdlRenderer, sdlFont),
      gameState(GameStateEnum::MENU), score(0), gameOver(false),
      lastDropTime(0), dropDelay(DROP_DELAY) {
}

GameEngine::~GameEngine() {
    // Components are automatically destroyed in reverse order of construction
}

void GameEngine::startNewGame() {
    board.clear();
    score = 0;
    gameOver = false;
    tetromino.initialize();
    lastDropTime = SDL_GetTicks();
    gameState = GameStateEnum::PLAYING;
}

bool GameEngine::checkCollision(int dx, int dy) {
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
                
                // Check boundary conditions
                if (boardX < 0 || boardX >= BOARD_WIDTH ||
                    boardY >= BOARD_HEIGHT) {
                    collision = true;
                    break;
                }
                
                // Check collision with placed blocks (only if in valid board area)
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
    inputHandler.pollEvents();
    
    if (gameState != GameStateEnum::PLAYING) {
        return;
    }
    
    // Get the current command
    GameCommand cmd = inputHandler.getCommand();
    
    switch (cmd) {
        case GameCommand::MOVE_LEFT:
            checkCollision(-1, 0);
            break;
        case GameCommand::MOVE_RIGHT:
            checkCollision(1, 0);
            break;
        case GameCommand::MOVE_DOWN:
            checkCollision(0, 1);
            break;
        case GameCommand::ROTATE:
            tetromino.rotate();
            // Note: Wall-kick could be implemented here if needed
            break;
        case GameCommand::HARD_DROP:
            // Drop piece until collision
            while (!checkCollision(0, 1)) {
                // Keep moving down
            }
            break;
        case GameCommand::QUIT:
            gameState = GameStateEnum::QUIT;
            break;
        default:
            break;
    }
}

void GameEngine::updateGameLogic(int currentTime) {
    if (gameState != GameStateEnum::PLAYING) {
        return;
    }
    
    // Auto-drop piece every DROP_DELAY milliseconds
    if (currentTime - lastDropTime > dropDelay) {
        lastDropTime = currentTime;
        
        // Try to move piece down
        bool collision = checkCollision(0, 1);
        
        if (collision) {
            // Piece has landed, lock it to the board
            lockPieceToBoard();
            
            // Clear complete lines and update score
            int linesCleared = board.clearCompleteLines();
            if (linesCleared > 0) {
                score += linesCleared * linesCleared * 100;
            }
            
            // Spawn next piece
            spawnNewPiece();
        }
    }
}

void GameEngine::update(int currentTime) {
    handleInput();
    updateGameLogic(currentTime);
}

void GameEngine::render() {
    switch (gameState) {
        case GameStateEnum::PLAYING:
            renderer.render(board, tetromino, score);
            break;
        case GameStateEnum::MENU:
            // Menu rendering should be handled separately
            renderer.renderMenu(0);
            break;
        case GameStateEnum::GAME_OVER:
            renderer.renderGameOver(score);
            break;
        case GameStateEnum::QUIT:
            // No rendering needed
            break;
    }
}
