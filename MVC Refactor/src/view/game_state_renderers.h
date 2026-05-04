/*
 * ============================================================================
 * FILE: game_state_renderers.h
 * PURPOSE: IGameStateRenderer implementations for different game states
 *          VIEW LAYER - Moved from ui/renderers/
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP) + Strategy Pattern
 *   Each game state has its own renderer. New UI screens/states can be added
 *   without modifying GameEngine or the main Renderer.
 * 
 * NEW STATES: To add a new game state screen, simply:
 *   1. Create a new class inheriting from IGameStateRenderer
 *   2. Implement the render() method accepting const Board&, const Tetromino&
 *   3. Instantiate it in GameEngine based on current state
 *   
 *   NO changes to GameEngine's main logic needed!
 * 
 * MVC COMPLIANCE:
 *   - All renderers accept const references to Model objects (read-only)
 *   - No mutation of Model state
 *   - Renderer manages SDL context internally
 * 
 * VIEW LAYER COMPONENT: Game state rendering implementations
 * ============================================================================
 */

#ifndef GAME_STATE_RENDERERS_H
#define GAME_STATE_RENDERERS_H

#include "interfaces/igame_state_renderer.h"
#include "renderer.h"

// Forward declarations
class Board;
class Tetromino;

// ============================================================================
// PLAYING STATE RENDERER
// ============================================================================
class PlayingRenderer : public IGameStateRenderer {
private:
    Renderer* renderer;
    
public:
    PlayingRenderer(Renderer* gameRenderer) : renderer(gameRenderer) {}
    
    void render(const Board& board, const Tetromino& tetromino, int score) override {
        renderer->render(board, tetromino, score);
    }
    
    const char* getName() const override { return "Playing"; }
};

// ============================================================================
// MENU STATE RENDERER
// ============================================================================
class MenuRenderer : public IGameStateRenderer {
private:
    Renderer* renderer;
    
public:
    MenuRenderer(Renderer* gameRenderer) : renderer(gameRenderer) {}
    
    void render(const Board& board, const Tetromino& tetromino, int score) override {
        // Menu doesn't need game state, just render the menu screen
        renderer->renderMenu(0);
    }
    
    const char* getName() const override { return "Menu"; }
};

// ============================================================================
// GAME OVER STATE RENDERER
// ============================================================================
class GameOverRenderer : public IGameStateRenderer {
private:
    Renderer* renderer;
    
public:
    GameOverRenderer(Renderer* gameRenderer) : renderer(gameRenderer) {}
    
    void render(const Board& board, const Tetromino& tetromino, int score) override {
        // Game over screen shows the final score
        renderer->renderGameOver(score);
    }
    
    const char* getName() const override { return "GameOver"; }
};

#endif // GAME_STATE_RENDERERS_H
