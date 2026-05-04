#ifndef GAME_OVER_SCREEN_H
#define GAME_OVER_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/*
 * CLASS: GameOverScreen
 * PURPOSE: Manages game over screen rendering
 * RESPONSIBILITY: Single - Render and manage game over UI
 * 
 * VIEW LAYER: UI screen implementation, no Model dependencies
 */
class GameOverScreen {
private:
    TTF_Font* font;
    SDL_Renderer* renderer;
    
    // Helper methods for rendering
    void drawBackground();
    void drawTitle();
    void drawScore(int final_score);
    void drawInstructions();
    
public:
    GameOverScreen(SDL_Renderer* sdl_renderer, TTF_Font* sdl_font);
    
    // Main rendering function
    void draw(int final_score);
};

#endif // GAME_OVER_SCREEN_H
