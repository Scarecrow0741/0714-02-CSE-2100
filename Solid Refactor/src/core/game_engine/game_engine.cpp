/*
 * ============================================================================
 * FILE: game_engine.cpp
 * PURPOSE: Implementation of GameEngine class (DIP-COMPLIANT)
 * 
 * DEPENDENCY INVERSION PRINCIPLE (DIP):
 *   - No longer creates InputHandler, Renderer, or ScoringStrategy internally
 *   - Receives all dependencies via constructor parameters
 *   - Depends only on abstractions (IInputProvider, ScoringStrategy)
 *   - Concrete implementations are created and wired in main.cpp (composition root)
 *   - All external dependencies are owned by GameEngine (via unique_ptr)
 * 
 * BENEFITS:
 *   - Easy to test: MockInputProvider, MockScoringStrategy can be injected
 *   - Easy to extend: New input/scoring/rendering implementations without changing GameEngine
 *   - Clear dependencies: Constructor shows exactly what GameEngine needs
 *   - Single responsibility: GameEngine orchestrates, doesn't create dependencies
 * ============================================================================
 */

#include "game_engine.h"
#include "../../ui/renderer/renderer.h"
#include "../../ui/renderers/game_state_renderers.h"

#define DROP_DELAY 500  // Milliseconds between automatic piece drops

GameEngine::GameEngine(
    std::unique_ptr<IInputProvider> inputProvider,
    std::unique_ptr<Renderer> renderer,
    std::unique_ptr<ScoringStrategy> scoringStrategy,
    std::unique_ptr<GameRenderer> playingRenderer,
    std::unique_ptr<GameRenderer> menuRenderer,
    std::unique_ptr<GameRenderer> gameOverRenderer
)
    : board(), tetromino(),
      inputProvider(std::move(inputProvider)),
      renderer(std::move(renderer)),
      scoringStrategy(std::move(scoringStrategy)),
      playingRenderer(std::move(playingRenderer)),
      menuRenderer(std::move(menuRenderer)),
      gameOverRenderer(std::move(gameOverRenderer)),
      gameState(GameStateEnum::MENU), score(0), level(1), gameOver(false),
      lastDropTime(0), dropDelay(DROP_DELAY) {
    
    // All components are now fully initialized by caller (main.cpp)
    // GameEngine just uses what it's given
}

GameEngine::~GameEngine() {
    // Components are automatically destroyed in reverse order of construction
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
            checkCollision(0, 1);
            break;
        case GameCommand::ROTATE:
            // LSP COMPLIANCE: rotate() is called uniformly for ALL shapes
            // NOTE: Do NOT add "if (type != O_PIECE) rotate()" logic!
            // This would violate LSP - all shapes must handle rotation identically
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
            
            // Clear complete lines and update score using STRATEGY PATTERN
            int linesCleared = board.clearCompleteLines();
            if (linesCleared > 0) {
                // Delegate scoring to strategy (OCP: no hardcoded scoring logic here!)
                int points = scoringStrategy->calculateScore(linesCleared, level, score);
                score += points;
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
    // Use STRATEGY PATTERN for rendering different game states
    // Renderer is now injected as a dependency, accessed via unique_ptr
    SDL_Renderer* sdlRenderer = renderer ? renderer->getSDLRenderer() : nullptr;
    
    if (!sdlRenderer) {
        return;  // No renderer available
    }
    
    switch (gameState) {
        case GameStateEnum::PLAYING:
            if (playingRenderer) {
                playingRenderer->render(sdlRenderer, board, tetromino, score);
            }
            break;
        case GameStateEnum::MENU:
            if (menuRenderer) {
                menuRenderer->render(sdlRenderer, board, tetromino, score);
            }
            break;
        case GameStateEnum::GAME_OVER:
            if (gameOverRenderer) {
                gameOverRenderer->render(sdlRenderer, board, tetromino, score);
            }
            break;
        case GameStateEnum::QUIT:
            // No rendering needed
            break;
    }
}
