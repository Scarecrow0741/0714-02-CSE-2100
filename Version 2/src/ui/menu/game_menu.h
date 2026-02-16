/**
 * @file ui/menu/game_menu.h
 * @brief Menu system and game over screen rendering
 *
 * Provides UI for main menu navigation and game over screen display.
 * Handles menu interaction including mouse hover effects and button
 * click detection.
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#ifndef TETRIS_GAME_MENU_H
#define TETRIS_GAME_MENU_H

#include "../../core/types/tetris_types.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Checks if a mouse click is within a menu button
 * 
 * Determines which menu option (if any) is under the given mouse coordinates.
 * Used for click detection during menu interaction.
 * 
 * @param x Mouse X coordinate (screen space)
 * @param y Mouse Y coordinate (screen space)
 * 
 * @return MENU_PLAY if click is on Play button,
 *         MENU_EXIT if click is on Exit button,
 *         -1 if no valid button is under cursor
 * 
 * @complexity O(1) - simple rectangle inclusion tests
 */
MenuOption Menu_CheckClickPosition(int x, int y);

/**
 * @brief Renders the main menu screen
 * 
 * Displays the Tetris title, Play/Exit options with visual effects,
 * button highlighting based on selection/hover state, and grid pattern
 * background.
 * 
 * @param renderer SDL renderer for drawing
 * @param font TTF font for text rendering
 * @param selected Currently selected menu option (affects highlight color)
 * 
 * @return None
 * 
 * @note Should be called once per frame during GAME_STATE_MENU
 * @note Calls SDL_RenderPresent at the end
 * @warning Requires valid renderer and font pointers
 */
void Menu_DrawMainMenu(SDL_Renderer *renderer, TTF_Font *font, MenuOption selected);

/**
 * @brief Renders the game over screen
 * 
 * Displays "GAME OVER" message, final score with background styling,
 * and instructions to return to menu. Grid pattern background included.
 * 
 * @param renderer SDL renderer for drawing
 * @param font TTF font for text rendering
 * @param final_score The score achieved in the completed game
 * 
 * @return None
 * 
 * @note Should be called once per frame during GAME_STATE_GAME_OVER
 * @note Calls SDL_RenderPresent at the end
 * @warning Requires valid renderer and font pointers
 */
void Menu_DrawGameOver(SDL_Renderer *renderer, TTF_Font *font, int final_score);

#endif // TETRIS_GAME_MENU_H
