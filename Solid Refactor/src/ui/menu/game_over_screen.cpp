/*
 * ============================================================================
 * FILE: game_over_screen.cpp
 * PURPOSE: Implementation of GameOverScreen class
 * RESPONSIBILITY: Game over screen rendering
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
    
    SDL_Color title_color = {255, 0, 0, 255};  // Bright red for GAME OVER
    
    SDL_Surface* gameover_surface = TTF_RenderText_Solid(font, "GAME OVER", title_color);
    if (gameover_surface) {
        SDL_Texture* gameover_texture = SDL_CreateTextureFromSurface(renderer, gameover_surface);
        SDL_Rect gameover_rect = {
            window_w/2 - (gameover_surface->w / 2),  // Centered horizontally
            (int)(window_h * 0.3),                   // 30% down from top
            gameover_surface->w,
            gameover_surface->h
        };
        SDL_RenderCopy(renderer, gameover_texture, NULL, &gameover_rect);
        SDL_DestroyTexture(gameover_texture);
        SDL_FreeSurface(gameover_surface);
    }
}

/*
 * FUNCTION: GameOverScreen::drawScore()
 * PURPOSE: Render the final score with background box
 */
void GameOverScreen::drawScore(int final_score) {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
    
    SDL_Color score_color = {255, 255, 255, 255};  // White for score
    
    char score_text[32];
    sprintf(score_text, "Final Score: %d", final_score);
    
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_text, score_color);
    if (score_surface) {
        SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
        
        // Draw background box for score (darker for contrast)
        SDL_Rect bg_rect = {
            window_w/2 - (score_surface->w / 2) - 20,
            (int)(window_h * 0.45) - 10,
            score_surface->w + 40,
            score_surface->h + 20
        };
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &bg_rect);
        
        // Draw score text
        SDL_Rect score_rect = {
            window_w/2 - (score_surface->w / 2),
            (int)(window_h * 0.45),
            score_surface->w,
            score_surface->h
        };
        SDL_RenderCopy(renderer, score_texture, NULL, &score_rect);
        SDL_DestroyTexture(score_texture);
        SDL_FreeSurface(score_surface);
    }
}

/*
 * FUNCTION: GameOverScreen::drawInstructions()
 * PURPOSE: Render instructions to return to menu
 */
void GameOverScreen::drawInstructions() {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
    
    SDL_Color instr_color = {255, 255, 255, 255};  // White for instructions
    
    SDL_Surface* press_surface = TTF_RenderText_Solid(font, "Press ENTER to return to menu", instr_color);
    if (press_surface) {
        SDL_Texture* press_texture = SDL_CreateTextureFromSurface(renderer, press_surface);
        SDL_Rect press_rect = {
            window_w/2 - (press_surface->w / 2),
            (int)(window_h * 0.6),
            press_surface->w,
            press_surface->h
        };
        SDL_RenderCopy(renderer, press_texture, NULL, &press_rect);
        SDL_DestroyTexture(press_texture);
        SDL_FreeSurface(press_surface);
    }
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
