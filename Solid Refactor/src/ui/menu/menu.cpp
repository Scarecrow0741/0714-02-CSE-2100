/*
 * ============================================================================
 * FILE: menu.cpp
 * PURPOSE: Menu system orchestration for Tetris (SRP-refactored)
 * 
 * This file now acts as a facade/orchestrator that delegates to:
 *   - Button class: Button rendering and hit detection
 *   - MenuScreen class: Main menu screen rendering
 *   - GameOverScreen class: Game over screen rendering
 * 
 * RESPONSIBILITY: Provide backward-compatible C-style interface
 * 
 * VISUAL ELEMENTS:
 *   - Styled buttons with shadows and borders
 *   - Hover effects (color changes when mouse over button)
 *   - Selected state highlighting
 *   - Grid background pattern
 *   - Colored text (green title, yellow selected, white text)
 * ============================================================================
 */

#include "../../include/tetris.h"
#include "menu_screen.h"
#include "game_over_screen.h"
#include <bits/stdc++.h>
using namespace std;

// Global instances for menu screens (initialized via setter functions)
static MenuScreen* g_menu_screen = nullptr;
static GameOverScreen* g_game_over_screen = nullptr;

/*
 * FUNCTION: init_menu_system()
 * PURPOSE: Initialize menu screen objects
 * Must be called before using draw_menu() or draw_game_over()
 */
void init_menu_system(SDL_Renderer* renderer, TTF_Font* font) {
    if (!g_menu_screen) {
        g_menu_screen = new MenuScreen(renderer, font);
    }
    if (!g_game_over_screen) {
        g_game_over_screen = new GameOverScreen(renderer, font);
    }
}

/*
 * FUNCTION: cleanup_menu_system()
 * PURPOSE: Clean up menu screen objects
 * Should be called during shutdown
 */
void cleanup_menu_system() {
    if (g_menu_screen) {
        delete g_menu_screen;
        g_menu_screen = nullptr;
    }
    if (g_game_over_screen) {
        delete g_game_over_screen;
        g_game_over_screen = nullptr;
    }
}

/*
 * FUNCTION: draw_button()
 * PURPOSE: DEPRECATED - Use Button class instead
 * 
 * Kept for backward compatibility only.
 * This function is no longer used internally.
 */
void draw_button(SDL_Renderer* renderer, SDL_Rect* rect, SDL_bool is_selected, SDL_bool is_hovered) {
    // This function is kept for backward compatibility but is not used internally.
    // The Button class now handles all button rendering.
    (void)renderer;   // Suppress unused parameter warning
    (void)rect;
    (void)is_selected;
    (void)is_hovered;
}

/*
 * FUNCTION: check_menu_click()
 * PURPOSE: DEPRECATED - Use MenuScreen::handleClick() instead
 * 
 * Kept for backward compatibility. Now delegates to MenuScreen class.
 * Returns MENU_PLAY as default if menu system not initialized.
 * 
 * PARAMETERS:
 *   x - Mouse cursor X coordinate
 *   y - Mouse cursor Y coordinate
 * 
 * RETURNS:
 *   Button clicked (MENU_PLAY or MENU_EXIT), or default if none
 */
MenuOption check_menu_click(int x, int y) {
    if (g_menu_screen) {
        return g_menu_screen->handleClick(x, y);
    }
    return MENU_PLAY;  // Default
}

/*
 * FUNCTION: draw_menu()
 * PURPOSE: Render the main menu screen
 * 
 * Now delegates to MenuScreen class for all rendering.
 * Requires init_menu_system() to be called first.
 * 
 * Displays:
 *   1. Grid background pattern (aesthetic)
 *   2. "TETRIS" title in bright green at top
 *   3. "Play" button with selection/hover effects
 *   4. "Exit" button with selection/hover effects
 * 
 * PARAMETERS:
 *   renderer - SDL renderer for drawing (superseded by init_menu_system)
 *   font - TTF font for rendering text (superseded by init_menu_system)
 *   selected - Currently selected menu option (MENU_PLAY or MENU_EXIT)
 */
void draw_menu(SDL_Renderer* renderer, TTF_Font* font, MenuOption selected) {
    // Initialize on first call
    if (!g_menu_screen) {
        init_menu_system(renderer, font);
    }
    
    if (g_menu_screen) {
        g_menu_screen->draw(selected);
    }
}

/*
 * FUNCTION: draw_game_over()
 * PURPOSE: Render the game over screen
 * 
 * Now delegates to GameOverScreen class for all rendering.
 * Requires init_menu_system() to be called first.
 * 
 * Displays:
 *   1. Red grid background pattern (indicating game over)
 *   2. "GAME OVER" text in large red letters
 *   3. "Final Score: X" with the player's score
 *   4. Instructions to press ENTER to return to menu
 * 
 * PARAMETERS:
 *   renderer - SDL renderer for drawing (superseded by init_menu_system)
 *   font - TTF font for rendering text (superseded by init_menu_system)
 *   final_score - The player's final score to display
 */
void draw_game_over(SDL_Renderer* renderer, TTF_Font* font, int final_score) {
    // Initialize on first call
    if (!g_game_over_screen) {
        init_menu_system(renderer, font);
    }
    
    if (g_game_over_screen) {
        g_game_over_screen->draw(final_score);
    }
}

