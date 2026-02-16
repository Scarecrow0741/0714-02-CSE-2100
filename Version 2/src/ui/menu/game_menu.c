/**
 * @file ui/menu/game_menu.c
 * @brief Menu system implementation
 *
 * Handles main menu and game over screen rendering with interactive elements.
 * Maintains button rectangles for hit detection and provides visual feedback
 * for menu state (selected, hovered).
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#include "game_menu.h"
#include <stdio.h>
#include <math.h>

/** Global button rectangles for hit detection */
static SDL_Rect g_playButtonRect = {0, 0, 0, 0};
static SDL_Rect g_exitButtonRect = {0, 0, 0, 0};

/**
 * @brief Draws a styled button with optional selection state
 * 
 * Creates a button with shadow, background color that changes based on
 * selection/hover state, and a colored border.
 * 
 * @param renderer SDL renderer
 * @param rect Button rectangle
 * @param isSelected Whether button is selected (affects color)
 * @param isHovered Whether mouse is over button (affects color)
 * 
 * @return None
 */
static void drawButton(SDL_Renderer *renderer, SDL_Rect *rect, 
                       SDL_bool isSelected, SDL_bool isHovered) {
    /* Draw shadow */
    SDL_Rect shadow = {rect->x + 3, rect->y + 3, rect->w, rect->h};
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderFillRect(renderer, &shadow);

    /* Draw button background with color based on state */
    int base_color = isSelected ? 80 : (isHovered ? 70 : 60);
    SDL_SetRenderDrawColor(renderer, 
        isSelected ? 60 : (isHovered ? 50 : 40),
        isSelected ? 60 : (isHovered ? 50 : 40),
        base_color,
        255);
    SDL_RenderFillRect(renderer, rect);

    /* Draw button border */
    int border_color = isSelected ? 140 : (isHovered ? 120 : 100);
    SDL_SetRenderDrawColor(renderer,
        isSelected ? 100 : (isHovered ? 80 : 60),
        isSelected ? 100 : (isHovered ? 80 : 60),
        border_color,
        255);
    SDL_RenderDrawRect(renderer, rect);
}

MenuOption Menu_CheckClickPosition(int x, int y) {
    /* Check if click is within Play button */
    if (x >= g_playButtonRect.x && x <= g_playButtonRect.x + g_playButtonRect.w &&
        y >= g_playButtonRect.y && y <= g_playButtonRect.y + g_playButtonRect.h) {
        return MENU_PLAY;
    }
    
    /* Check if click is within Exit button */
    if (x >= g_exitButtonRect.x && x <= g_exitButtonRect.x + g_exitButtonRect.w &&
        y >= g_exitButtonRect.y && y <= g_exitButtonRect.y + g_exitButtonRect.h) {
        return MENU_EXIT;
    }
    
    return -1; /* No button clicked */
}

void Menu_DrawMainMenu(SDL_Renderer *renderer, TTF_Font *font, MenuOption selected) {
    /* Get window dimensions */
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);

    /* Clear background */
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
    SDL_RenderClear(renderer);

    /* Draw grid pattern background */
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    for (int x = 0; x < window_w; x += 40) {
        SDL_RenderDrawLine(renderer, x, 0, x, window_h);
    }
    for (int y = 0; y < window_h; y += 40) {
        SDL_RenderDrawLine(renderer, 0, y, window_w, y);
    }

    /* Color palette */
    SDL_Color title_color = {50, 255, 150, 255};      /* Bright green */
    SDL_Color text_color = {220, 220, 220, 255};      /* Off-white */
    SDL_Color selected_color = {255, 215, 0, 255};    /* Golden yellow */

    /* Render title "TETRIS" */
    SDL_Surface *title_surface = TTF_RenderText_Solid(font, "TETRIS", title_color);
    if (title_surface) {
        SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
        SDL_Rect title_rect = {
            window_w / 2 - (title_surface->w / 2),
            window_h * 0.2,
            title_surface->w,
            title_surface->h
        };
        SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
        SDL_DestroyTexture(title_texture);
        SDL_FreeSurface(title_surface);
    }

    /* Get current mouse position for hover effect */
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    /* Render "Play" option */
    SDL_Color play_color = (selected == MENU_PLAY) ? selected_color : text_color;
    SDL_Surface *play_surface = TTF_RenderText_Solid(font, "Play", play_color);
    if (play_surface) {
        SDL_Texture *play_texture = SDL_CreateTextureFromSurface(renderer, play_surface);
        
        /* Update button rect for hit detection */
        g_playButtonRect.x = window_w / 2 - (play_surface->w / 2) - 40;
        g_playButtonRect.y = window_h * 0.5 - 15;
        g_playButtonRect.w = play_surface->w + 80;
        g_playButtonRect.h = play_surface->h + 30;

        /* Check if mouse is hovering */
        SDL_bool isHovered = (Menu_CheckClickPosition(mouse_x, mouse_y) == MENU_PLAY);
        drawButton(renderer, &g_playButtonRect, selected == MENU_PLAY, isHovered);

        /* Draw text */
        SDL_Rect play_rect = {
            window_w / 2 - (play_surface->w / 2),
            window_h * 0.5,
            play_surface->w,
            play_surface->h
        };
        SDL_RenderCopy(renderer, play_texture, NULL, &play_rect);
        SDL_DestroyTexture(play_texture);
        SDL_FreeSurface(play_surface);
    }

    /* Render "Exit" option */
    SDL_Color exit_color = (selected == MENU_EXIT) ? selected_color : text_color;
    SDL_Surface *exit_surface = TTF_RenderText_Solid(font, "Exit", exit_color);
    if (exit_surface) {
        SDL_Texture *exit_texture = SDL_CreateTextureFromSurface(renderer, exit_surface);
        
        /* Update button rect for hit detection */
        g_exitButtonRect.x = window_w / 2 - (exit_surface->w / 2) - 40;
        g_exitButtonRect.y = window_h * 0.65 - 15;
        g_exitButtonRect.w = exit_surface->w + 80;
        g_exitButtonRect.h = exit_surface->h + 30;

        /* Check if mouse is hovering */
        SDL_bool isHovered = (Menu_CheckClickPosition(mouse_x, mouse_y) == MENU_EXIT);
        drawButton(renderer, &g_exitButtonRect, selected == MENU_EXIT, isHovered);

        /* Draw text */
        SDL_Rect exit_rect = {
            window_w / 2 - (exit_surface->w / 2),
            window_h * 0.65,
            exit_surface->w,
            exit_surface->h
        };
        SDL_RenderCopy(renderer, exit_texture, NULL, &exit_rect);
        SDL_DestroyTexture(exit_texture);
        SDL_FreeSurface(exit_surface);
    }

    SDL_RenderPresent(renderer);
}

void Menu_DrawGameOver(SDL_Renderer *renderer, TTF_Font *font, int final_score) {
    /* Get window dimensions */
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);

    /* Clear background (dark red) */
    SDL_SetRenderDrawColor(renderer, 20, 0, 0, 255);
    SDL_RenderClear(renderer);

    /* Draw grid pattern background */
    SDL_SetRenderDrawColor(renderer, 40, 20, 20, 255);
    for (int x = 0; x < window_w; x += 40) {
        SDL_RenderDrawLine(renderer, x, 0, x, window_h);
    }
    for (int y = 0; y < window_h; y += 40) {
        SDL_RenderDrawLine(renderer, 0, y, window_w, y);
    }

    /* Color palette */
    SDL_Color title_color = {255, 0, 0, 255};     /* Red */
    SDL_Color score_color = {255, 255, 255, 255}; /* White */

    /* Draw "GAME OVER" text */
    SDL_Surface *gameover_surface = TTF_RenderText_Solid(font, "GAME OVER", title_color);
    if (gameover_surface) {
        SDL_Texture *gameover_texture = SDL_CreateTextureFromSurface(renderer, gameover_surface);
        SDL_Rect gameover_rect = {
            window_w / 2 - (gameover_surface->w / 2),
            window_h * 0.3,
            gameover_surface->w,
            gameover_surface->h
        };
        SDL_RenderCopy(renderer, gameover_texture, NULL, &gameover_rect);
        SDL_DestroyTexture(gameover_texture);
        SDL_FreeSurface(gameover_surface);
    }

    /* Draw final score with background box */
    char score_text[32];
    sprintf(score_text, "Final Score: %d", final_score);
    SDL_Surface *score_surface = TTF_RenderText_Solid(font, score_text, score_color);
    if (score_surface) {
        SDL_Texture *score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
        
        /* Score background box */
        SDL_Rect bg_rect = {
            window_w / 2 - (score_surface->w / 2) - 20,
            window_h * 0.45 - 10,
            score_surface->w + 40,
            score_surface->h + 20
        };
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderFillRect(renderer, &bg_rect);

        /* Score text */
        SDL_Rect score_rect = {
            window_w / 2 - (score_surface->w / 2),
            window_h * 0.45,
            score_surface->w,
            score_surface->h
        };
        SDL_RenderCopy(renderer, score_texture, NULL, &score_rect);
        SDL_DestroyTexture(score_texture);
        SDL_FreeSurface(score_surface);
    }

    /* Draw instruction text */
    SDL_Surface *press_surface = TTF_RenderText_Solid(font, 
        "Press ENTER to return to menu", score_color);
    if (press_surface) {
        SDL_Texture *press_texture = SDL_CreateTextureFromSurface(renderer, press_surface);
        SDL_Rect press_rect = {
            window_w / 2 - (press_surface->w / 2),
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
