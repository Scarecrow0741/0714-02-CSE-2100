/**
 * @file ui/sdl/sdl_renderer.h
 * @brief SDL2-based graphical rendering for gameplay
 *
 * Provides functions to render the game board, falling piece, HUD,
 * and grid. Depends only on SDL2 and GameState, maintaining separation
 * from core game logic (Dependency Inversion Principle).
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#ifndef TETRIS_SDL_RENDERER_H
#define TETRIS_SDL_RENDERER_H

#include "../../core/types/tetris_types.h"

/**
 * @brief Renders the current gameplay frame  
 * 
 * Draws the complete game screen including:
 * - Game board background and border
 * - Placed blocks with per-type colors
 * - Currently falling tetromino
 * - Grid lines for visual clarity
 * - HUD panel with score display
 * - Next piece preview
 * - Console score output for debugging
 * 
 * @param game Pointer to the GameState structure (must be in GAME_STATE_PLAYING)
 * 
 * @return None
 * 
 * @complexity O(board_width * board_height + 16) for block rendering
 * 
 * @note Must be called every frame during gameplay
 * @note Calls SDL_RenderPresent at the end to display frame
 * @warning Assumes game->renderer is valid
 */
void SDLRenderer_DrawGameplay(GameState *game);

#endif // TETRIS_SDL_RENDERER_H
