/*
 * ============================================================================
 * FILE: game_state_renderers.h
 * PURPOSE: GameRenderer implementations for different game states
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP) + Strategy Pattern
 *   Each game state has its own renderer. New UI screens/states can be added
 *   without modifying GameEngine or the main Renderer.
 * 
 * NEW STATES: To add a new game state screen, simply:
 *   1. Create a new class inheriting from GameRenderer
 *   2. Implement the render() method
 *   3. Instantiate it in GameEngine based on current state
 *   
 *   NO changes to GameEngine's main logic needed!
 * ============================================================================
 */

#ifndef GAME_STATE_RENDERERS_H
#define GAME_STATE_RENDERERS_H

#include "../renderer/renderer.h"
#include "../../core/interfaces/game_renderer.h"
#include <SDL2/SDL_ttf.h>

// ============================================================================
// PLAYING STATE RENDERER
// ============================================================================
class PlayingRenderer : public GameRenderer {
private:
    Renderer* renderer;
    
public:
    PlayingRenderer(Renderer* gameRenderer) : renderer(gameRenderer) {}
    
    void render(SDL_Renderer* sdlRenderer, const Board& board, 
               const Tetromino& tetromino, int score) override {
        renderer->render(board, tetromino, score);
    }
    
    const char* getName() const override { return "Playing"; }
};

// ============================================================================
// MENU STATE RENDERER
// ============================================================================
class MenuRenderer : public GameRenderer {
private:
    Renderer* renderer;
    
public:
    MenuRenderer(Renderer* gameRenderer) : renderer(gameRenderer) {}
    
    void render(SDL_Renderer* sdlRenderer, const Board& board,
               const Tetromino& tetromino, int score) override {
        renderer->renderMenu(0);
    }
    
    const char* getName() const override { return "Menu"; }
};

// ============================================================================
// GAME OVER STATE RENDERER
// ============================================================================
class GameOverRenderer : public GameRenderer {
private:
    Renderer* renderer;
    
public:
    GameOverRenderer(Renderer* gameRenderer) : renderer(gameRenderer) {}
    
    void render(SDL_Renderer* sdlRenderer, const Board& board,
               const Tetromino& tetromino, int score) override {
        renderer->renderGameOver(score);
    }
    
    const char* getName() const override { return "GameOver"; }
};

#endif // GAME_STATE_RENDERERS_H
