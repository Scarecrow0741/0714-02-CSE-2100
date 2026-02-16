#include "tetris.h"
#include <stdio.h>
#include <math.h>

// Add menu button hit detection
SDL_Rect play_button_rect = {0, 0, 0, 0};
SDL_Rect exit_button_rect = {0, 0, 0, 0};

// Helper function to draw a stylized button
void draw_button(SDL_Renderer* renderer, SDL_Rect* rect, SDL_bool isSelected, SDL_bool isHovered) {
    // Draw shadow
    SDL_Rect shadow = {rect->x + 3, rect->y + 3, rect->w, rect->h};
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderFillRect(renderer, &shadow);

    // Draw button background
    SDL_SetRenderDrawColor(renderer, 
        isSelected ? 60 : (isHovered ? 50 : 40),
        isSelected ? 60 : (isHovered ? 50 : 40),
        isSelected ? 80 : (isHovered ? 70 : 60),
        255);
    SDL_RenderFillRect(renderer, rect);

    // Draw button border
    SDL_SetRenderDrawColor(renderer, 
        isSelected ? 100 : (isHovered ? 80 : 60),
        isSelected ? 100 : (isHovered ? 80 : 60),
        isSelected ? 140 : (isHovered ? 120 : 100),
        255);
    SDL_RenderDrawRect(renderer, rect);
}

MenuOption check_menu_click(int x, int y) {
    if (x >= play_button_rect.x && x <= play_button_rect.x + play_button_rect.w &&
        y >= play_button_rect.y && y <= play_button_rect.y + play_button_rect.h) {
        return MENU_PLAY;
    }
    if (x >= exit_button_rect.x && x <= exit_button_rect.x + exit_button_rect.w &&
        y >= exit_button_rect.y && y <= exit_button_rect.y + exit_button_rect.h) {
        return MENU_EXIT;
    }
    return -1;
}

void draw_menu(SDL_Renderer* renderer, TTF_Font* font, MenuOption selected) {
    // Get window size
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);

    // Set solid background color
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
    SDL_RenderClear(renderer);

    // Draw simple grid pattern
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    for(int x = 0; x < window_w; x += 40) {
        SDL_RenderDrawLine(renderer, x, 0, x, window_h);
    }
    for(int y = 0; y < window_h; y += 40) {
        SDL_RenderDrawLine(renderer, 0, y, window_w, y);
    }

    // Enhanced colors
    SDL_Color title_color = {50, 255, 150, 255}; // Bright green
    SDL_Color text_color = {220, 220, 220, 255}; // Slightly off-white
    SDL_Color selected_color = {255, 215, 0, 255}; // Golden yellow

    // Render main title
    SDL_Surface* title_surface = TTF_RenderText_Solid(font, "TETRIS", title_color);
    if (title_surface) {
        SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
        SDL_Rect title_rect = {
            window_w/2 - (title_surface->w / 2),
            window_h * 0.2,
            title_surface->w,
            title_surface->h
        };
        SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
        SDL_DestroyTexture(title_texture);
        SDL_FreeSurface(title_surface);
    }

    // Get mouse position for hover effect
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // Render "Play" option with enhanced button
    SDL_Surface* play_surface = TTF_RenderText_Solid(font, "Play", 
        selected == MENU_PLAY ? selected_color : text_color);
    if (play_surface) {
        SDL_Texture* play_texture = SDL_CreateTextureFromSurface(renderer, play_surface);
        
        // Update button rect for hit detection
        play_button_rect.x = window_w/2 - (play_surface->w / 2) - 40;
        play_button_rect.y = window_h * 0.5 - 15;
        play_button_rect.w = play_surface->w + 80;
        play_button_rect.h = play_surface->h + 30;

        // Draw button with hover effect
        SDL_bool isHovered = (check_menu_click(mouse_x, mouse_y) == MENU_PLAY);
        draw_button(renderer, &play_button_rect, selected == MENU_PLAY, isHovered);

        // Draw text
        SDL_Rect play_rect = {
            window_w/2 - (play_surface->w / 2),
            window_h * 0.5,
            play_surface->w,
            play_surface->h
        };
        SDL_RenderCopy(renderer, play_texture, NULL, &play_rect);
        SDL_DestroyTexture(play_texture);
        SDL_FreeSurface(play_surface);
    }

    // Render "Exit" option with enhanced button
    SDL_Surface* exit_surface = TTF_RenderText_Solid(font, "Exit", 
        selected == MENU_EXIT ? selected_color : text_color);
    if (exit_surface) {
        SDL_Texture* exit_texture = SDL_CreateTextureFromSurface(renderer, exit_surface);
        
        // Update button rect for hit detection
        exit_button_rect.x = window_w/2 - (exit_surface->w / 2) - 40;
        exit_button_rect.y = window_h * 0.65 - 15;  // Increased spacing
        exit_button_rect.w = exit_surface->w + 80;
        exit_button_rect.h = exit_surface->h + 30;

        // Draw button with hover effect
        SDL_bool isHovered = (check_menu_click(mouse_x, mouse_y) == MENU_EXIT);
        draw_button(renderer, &exit_button_rect, selected == MENU_EXIT, isHovered);

        // Draw text
        SDL_Rect exit_rect = {
            window_w/2 - (exit_surface->w / 2),
            window_h * 0.65,  // Increased spacing
            exit_surface->w,
            exit_surface->h
        };
        SDL_RenderCopy(renderer, exit_texture, NULL, &exit_rect);
        SDL_DestroyTexture(exit_texture);
        SDL_FreeSurface(exit_surface);
    }

    SDL_RenderPresent(renderer);
}

void draw_game_over(SDL_Renderer* renderer, TTF_Font* font, int final_score) {
    // Get window size
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);

    // Set solid background color
    SDL_SetRenderDrawColor(renderer, 20, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw static grid pattern
    SDL_SetRenderDrawColor(renderer, 40, 20, 20, 255);
    for(int x = 0; x < window_w; x += 40) {
        SDL_RenderDrawLine(renderer, x, 0, x, window_h);
    }
    for(int y = 0; y < window_h; y += 40) {
        SDL_RenderDrawLine(renderer, 0, y, window_w, y);
    }

    SDL_Color title_color = {255, 0, 0, 255}; // Red
    SDL_Color score_color = {255, 255, 255, 255}; // White

    // Draw "GAME OVER" text
    SDL_Surface* gameover_surface = TTF_RenderText_Solid(font, "GAME OVER", title_color);
    if (gameover_surface) {
        SDL_Texture* gameover_texture = SDL_CreateTextureFromSurface(renderer, gameover_surface);
        SDL_Rect gameover_rect = {
            window_w/2 - (gameover_surface->w / 2),
            window_h * 0.3,
            gameover_surface->w,
            gameover_surface->h
        };
        SDL_RenderCopy(renderer, gameover_texture, NULL, &gameover_rect);
        SDL_DestroyTexture(gameover_texture);
        SDL_FreeSurface(gameover_surface);
    }

    // Draw final score with background
    char score_text[32];
    sprintf(score_text, "Final Score: %d", final_score);
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_text, score_color);
    if (score_surface) {
        SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
        
        // Score background
        SDL_Rect bg_rect = {
            window_w/2 - (score_surface->w / 2) - 20,
            window_h * 0.45 - 10,
            score_surface->w + 40,
            score_surface->h + 20
        };
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &bg_rect);
        
        SDL_Rect score_rect = {
            window_w/2 - (score_surface->w / 2),
            window_h * 0.45,
            score_surface->w,
            score_surface->h
        };
        SDL_RenderCopy(renderer, score_texture, NULL, &score_rect);
        SDL_DestroyTexture(score_texture);
        SDL_FreeSurface(score_surface);
    }

    // Draw "Press ENTER to return to menu" text
    SDL_Surface* press_surface = TTF_RenderText_Solid(font, "Press ENTER to return to menu", score_color);
    if (press_surface) {
        SDL_Texture* press_texture = SDL_CreateTextureFromSurface(renderer, press_surface);
        SDL_Rect press_rect = {
            window_w/2 - (press_surface->w / 2),
            window_h * 0.6,
            press_surface->w,
            press_surface->h
        };
        SDL_RenderCopy(renderer, press_texture, NULL, &press_rect);
        SDL_DestroyTexture(press_texture);
        SDL_FreeSurface(press_surface);
    }

    SDL_RenderPresent(renderer);
}