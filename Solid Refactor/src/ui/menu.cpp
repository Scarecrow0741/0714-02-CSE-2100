/*
 * ============================================================================
 * FILE: menu.cpp
 * PURPOSE: Menu system and UI rendering for Tetris
 * 
 * This file handles:
 *   - Main menu display with Play/Exit buttons
 *   - Game over screen with final score display
 *   - Button drawing with styling (shadow, hover effects, colors)
 *   - Button click detection based on mouse position
 *   - Text rendering using SDL2_TTF for menu labels
 *   - Grid pattern backgrounds for visual appeal
 * 
 * VISUAL ELEMENTS:
 *   - Styled buttons with shadows and borders
 *   - Hover effects (color changes when mouse over button)
 *   - Selected state highlighting
 *   - Grid background pattern
 *   - Colored text (green title, yellow selected, white text)
 * ============================================================================
 */

#include "../include/tetris.h"
#include <bits/stdc++.h>
using namespace std;

// Global button rectangles for menu hit detection
// These store the screen coordinates and dimensions of menu buttons
SDL_Rect g_play_button_rect = {0, 0, 0, 0};
SDL_Rect g_exit_button_rect = {0, 0, 0, 0};

/*
 * FUNCTION: draw_button()
 * PURPOSE: Draw a single menu button with styling
 * 
 * Features:
 *   - Shadow effect (dark rectangle offset by 3 pixels)
 *   - Color-coded based on selection state (selected/hovered/normal)
 *   - Border outline for definition
 *   - Three-color palette: darker when selected, slightly lighter when hovered
 * 
 * PARAMETERS:
 *   renderer - SDL renderer for drawing
 *   rect - Button's screen position and dimensions
 *   is_selected - Whether button is currently selected (true = highlighted)
 *   is_hovered - Whether mouse is over button (true = brightened)
 */
void draw_button(SDL_Renderer* renderer, SDL_Rect* rect, SDL_bool is_selected, SDL_bool is_hovered) {
    // Draw shadow effect (creates depth illusion)
    SDL_Rect shadow = {rect->x + 3, rect->y + 3, rect->w, rect->h};
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);  // Dark gray shadow
    SDL_RenderFillRect(renderer, &shadow);

    // Draw button background with color based on state
    // Selected buttons are brightest blue, hovered are medium, normal are darkest
    SDL_SetRenderDrawColor(renderer, 
        is_selected ? 60 : (is_hovered ? 50 : 40),      // R component
        is_selected ? 60 : (is_hovered ? 50 : 40),      // G component
        is_selected ? 80 : (is_hovered ? 70 : 60),      // B component
        255);
    SDL_RenderFillRect(renderer, rect);

    // Draw button border for visual definition
    SDL_SetRenderDrawColor(renderer, 
        is_selected ? 100 : (is_hovered ? 80 : 60),     // R component
        is_selected ? 100 : (is_hovered ? 80 : 60),     // G component
        is_selected ? 140 : (is_hovered ? 120 : 100),   // B component
        255);
    SDL_RenderDrawRect(renderer, rect);
}

/*
 * FUNCTION: check_menu_click()
 * PURPOSE: Determine which menu button (if any) was clicked
 * 
 * Uses bounding box collision detection to check if mouse coordinates
 * fall within either the Play or Exit button rectangle.
 * Returns the menu option corresponding to the clicked button,
 * or defaults to MENU_PLAY if no button clicked.
 * 
 * PARAMETERS:
 *   x - Mouse cursor X coordinate
 *   y - Mouse cursor Y coordinate
 * 
 * RETURNS:
 *   MENU_PLAY if Play button clicked,
 *   MENU_EXIT if Exit button clicked,
 *   MENU_PLAY (default) if neither button clicked
 */
MenuOption check_menu_click(int x, int y) {
    // Check if click is within Play button bounds
    if (x >= g_play_button_rect.x && x <= g_play_button_rect.x + g_play_button_rect.w &&
        y >= g_play_button_rect.y && y <= g_play_button_rect.y + g_play_button_rect.h) {
        return MENU_PLAY;
    }
    
    // Check if click is within Exit button bounds
    if (x >= g_exit_button_rect.x && x <= g_exit_button_rect.x + g_exit_button_rect.w &&
        y >= g_exit_button_rect.y && y <= g_exit_button_rect.y + g_exit_button_rect.h) {
        return MENU_EXIT;
    }
    
    // No button clicked, return default
    return MENU_PLAY;
}

/*
 * FUNCTION: draw_menu()
 * PURPOSE: Render the main menu screen
 * 
 * Displays:
 *   1. Grid background pattern (aesthetic)
 *   2. "TETRIS" title in bright green at top
 *   3. "Play" button with selection/hover effects
 *   4. "Exit" button with selection/hover effects
 *   5. Updates button rectangles for hit detection
 * 
 * The function uses the current mouse position to determine
 * hover state and apply visual feedback to buttons.
 * 
 * PARAMETERS:
 *   renderer - SDL renderer for drawing
 *   font - TTF font for rendering text
 *   selected - Currently selected menu option (MENU_PLAY or MENU_EXIT)
 */
void draw_menu(SDL_Renderer* renderer, TTF_Font* font, MenuOption selected) {
    // Get current window dimensions
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);

    // Clear screen with dark blue background
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
    SDL_RenderClear(renderer);

    // Draw grid pattern background (aesthetic element)
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    for(int x = 0; x < window_w; x += 40) {
        SDL_RenderDrawLine(renderer, x, 0, x, window_h);  // Vertical lines
    }
    for(int y = 0; y < window_h; y += 40) {
        SDL_RenderDrawLine(renderer, 0, y, window_w, y);  // Horizontal lines
    }

    // Define color palettes for different text elements
    SDL_Color title_color = {50, 255, 150, 255};        // Bright green for title
    SDL_Color text_color = {220, 220, 220, 255};        // Off-white for normal text
    SDL_Color selected_color = {255, 215, 0, 255};      // Golden yellow for selected

    // ===== RENDER TITLE =====
    SDL_Surface* title_surface = TTF_RenderText_Solid(font, "TETRIS", title_color);
    if (title_surface) {
        SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
        SDL_Rect title_rect = {
            window_w/2 - (title_surface->w / 2),  // Centered horizontally
            (int)(window_h * 0.2),                 // 20% down from top
            title_surface->w,
            title_surface->h
        };
        SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
        SDL_DestroyTexture(title_texture);
        SDL_FreeSurface(title_surface);
    }

    // Get current mouse position for hover detection
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // ===== RENDER PLAY BUTTON =====
    // Choose text color based on selection state
    SDL_Surface* play_surface = TTF_RenderText_Solid(font, "Play", 
        selected == MENU_PLAY ? selected_color : text_color);
    if (play_surface) {
        SDL_Texture* play_texture = SDL_CreateTextureFromSurface(renderer, play_surface);
        
        // Update the global button rectangle for hit detection
        g_play_button_rect.x = window_w/2 - (play_surface->w / 2) - 40;  // Padding: 40 pixels
        g_play_button_rect.y = window_h * 0.5 - 15;                       // Centered vertically
        g_play_button_rect.w = play_surface->w + 80;
        g_play_button_rect.h = play_surface->h + 30;

        // Determine if button is being hovered over
        SDL_bool is_hovered = (SDL_bool)(check_menu_click(mouse_x, mouse_y) == MENU_PLAY);
        
        // Draw button with styling
        draw_button(renderer, &g_play_button_rect, (SDL_bool)(selected == MENU_PLAY), is_hovered);

        // Draw button text
        SDL_Rect play_rect = {
            window_w/2 - (play_surface->w / 2),
            (int)(window_h * 0.5),
            play_surface->w,
            play_surface->h
        };
        SDL_RenderCopy(renderer, play_texture, NULL, &play_rect);
        SDL_DestroyTexture(play_texture);
        SDL_FreeSurface(play_surface);
    }

    // ===== RENDER EXIT BUTTON =====
    SDL_Surface* exit_surface = TTF_RenderText_Solid(font, "Exit", 
        selected == MENU_EXIT ? selected_color : text_color);
    if (exit_surface) {
        SDL_Texture* exit_texture = SDL_CreateTextureFromSurface(renderer, exit_surface);
        
        // Update the global button rectangle for hit detection
        g_exit_button_rect.x = window_w/2 - (exit_surface->w / 2) - 40;
        g_exit_button_rect.y = window_h * 0.65 - 15;  // Positioned lower than Play button
        g_exit_button_rect.w = exit_surface->w + 80;
        g_exit_button_rect.h = exit_surface->h + 30;

        // Determine if button is being hovered over
        SDL_bool is_hovered = (SDL_bool)(check_menu_click(mouse_x, mouse_y) == MENU_EXIT);
        
        // Draw button with styling
        draw_button(renderer, &g_exit_button_rect, (SDL_bool)(selected == MENU_EXIT), is_hovered);

        // Draw button text
        SDL_Rect exit_rect = {
            window_w/2 - (exit_surface->w / 2),
            window_h * 0.65,
            exit_surface->w,
            exit_surface->h
        };
        SDL_RenderCopy(renderer, exit_texture, NULL, &exit_rect);
        SDL_DestroyTexture(exit_texture);
        SDL_FreeSurface(exit_surface);
    }

    // Display the rendered frame
    SDL_RenderPresent(renderer);
}

/*
 * FUNCTION: draw_game_over()
 * PURPOSE: Render the game over screen
 * 
 * Displays:
 *   1. Red grid background pattern (indicating game over)
 *   2. "GAME OVER" text in large red letters
 *   3. "Final Score: X" with the player's score
 *   4. Instructions to press ENTER to return to menu
 * 
 * PARAMETERS:
 *   renderer - SDL renderer for drawing
 *   font - TTF font for rendering text
 *   final_score - The player's final score to display
 */
void draw_game_over(SDL_Renderer* renderer, TTF_Font* font, int final_score) {
    // Get current window dimensions
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

    // Define colors for game over screen
    SDL_Color title_color = {255, 0, 0, 255};      // Bright red for GAME OVER
    SDL_Color score_color = {255, 255, 255, 255}; // White for score and instructions

    // ===== RENDER "GAME OVER" TEXT =====
    SDL_Surface* gameover_surface = TTF_RenderText_Solid(font, "GAME OVER", title_color);
    if (gameover_surface) {
        SDL_Texture* gameover_texture = SDL_CreateTextureFromSurface(renderer, gameover_surface);
        SDL_Rect gameover_rect = {
            window_w/2 - (gameover_surface->w / 2),  // Centered horizontally
            window_h * 0.3,                           // 30% down from top
            gameover_surface->w,
            gameover_surface->h
        };
        SDL_RenderCopy(renderer, gameover_texture, NULL, &gameover_rect);
        SDL_DestroyTexture(gameover_texture);
        SDL_FreeSurface(gameover_surface);
    }

    // ===== RENDER FINAL SCORE =====
    char score_text[32];
    sprintf(score_text, "Final Score: %d", final_score);
    
    SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_text, score_color);
    if (score_surface) {
        SDL_Texture* score_texture = SDL_CreateTextureFromSurface(renderer, score_surface);
        
        // Draw background box for score (darker for contrast)
        SDL_Rect bg_rect = {
            window_w/2 - (score_surface->w / 2) - 20,
            window_h * 0.45 - 10,
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

    // ===== RENDER INSTRUCTIONS =====
    SDL_Surface* press_surface = TTF_RenderText_Solid(font, "Press ENTER to return to menu", score_color);
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

    // Display the rendered frame
    SDL_RenderPresent(renderer);
}

