/*
 * ============================================================================
 * FILE: game_over_screen.cpp
 * PURPOSE: Implementation of GameOverScreen class
 * RESPONSIBILITY: Game over screen rendering
 * 
 * VIEW LAYER: UI screen implementation
 * ============================================================================
 */

#include "game_over_screen.h"
#include <cstdio>

GameOverScreen::GameOverScreen(SDL_Renderer* sdl_renderer, TTF_Font* sdl_font)
    : renderer(sdl_renderer), font(sdl_font) {
}

/*
 * FUNCTION: GameOverScreen::drawBackground()
 * PURPOSE: Render background with red-tinted grid pattern
 */
void GameOverScreen::drawBackground() {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);

    // Clear screen with dark red background (game over theme)
    SDL_SetRenderDrawColor(renderer, 20, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw grid pattern in reddish tone
    SDL_SetRenderDrawColor(renderer, 40, 20, 20, 255);
    for(int x = 0; x < window_w; x += 40) {
        SDL_RenderDrawLine(renderer, x, 0, x, window_h);
    }
    for(int y = 0; y < window_h; y += 40) {
        SDL_RenderDrawLine(renderer, 0, y, window_w, y);
    }
}

/*
 * FUNCTION: GameOverScreen::drawTitle()
 * PURPOSE: Render the "GAME OVER" title in red
 */
void GameOverScreen::drawTitle() {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
    
    // Simple game over box (text rendering disabled)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect title_box = {
        window_w/2 - 100,
        (int)(window_h * 0.3),
        200,
        50
    };
    SDL_RenderDrawRect(renderer, &title_box);
}

/*
 * FUNCTION: GameOverScreen::drawScore()
 * PURPOSE: Render the final score with background box
 */
void GameOverScreen::drawScore(int final_score) {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
    
    // Simple score box (text rendering disabled)
    SDL_Rect score_box = {
        window_w/2 - 150,
        (int)(window_h * 0.45),
        300,
        50
    };
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderFillRect(renderer, &score_box);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &score_box);
}

/*
 * FUNCTION: GameOverScreen::drawInstructions()
 * PURPOSE: Render instructions to return to menu
 */
void GameOverScreen::drawInstructions() {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
    
    // Simple instructions box (text rendering disabled)
    SDL_Rect instr_box = {
        window_w/2 - 150,
        (int)(window_h * 0.6),
        300,
        50
    };
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderDrawRect(renderer, &instr_box);
}

/*
 * FUNCTION: GameOverScreen::draw()
 * PURPOSE: Render the complete game over screen
 */
void GameOverScreen::draw(int final_score) {
    drawBackground();
    drawTitle();
    drawScore(final_score);
    drawInstructions();
    
    // Display the rendered frame
    SDL_RenderPresent(renderer);
}
