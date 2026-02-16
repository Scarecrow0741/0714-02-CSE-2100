/**
 * @file core/types/tetris_types.h
 * @brief Type definitions and structures for the Tetris game engine
 *
 * This header defines all fundamental data types used throughout the Tetris engine,
 * including game state, screen states, menu options, and core game structures.
 * It follows the separation of concerns principle by centralizing all type definitions.
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#ifndef TETRIS_TYPES_H
#define TETRIS_TYPES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Enumeration for in-game screen states
 * 
 * Represents the different screens the application can display:
 * - GAME_STATE_MENU: Main menu screen
 * - GAME_STATE_PLAYING: Active gameplay screen  
 * - GAME_STATE_GAME_OVER: Game over/end screen
 */
typedef enum {
    GAME_STATE_MENU,      /**< Main menu screen state */
    GAME_STATE_PLAYING,   /**< Active gameplay state */
    GAME_STATE_GAME_OVER  /**< Game over state */
} GameScreenState;

/**
 * @brief Enumeration for menu options
 * 
 * Represents selectable options in the main menu
 */
typedef enum {
    MENU_PLAY = 0,   /**< Play game option */
    MENU_EXIT = 1    /**< Exit application option */
} MenuOption;

/**
 * @brief Main game state structure
 * 
 * Encapsulates all state required to maintain the game, including:
 * - Board representation (game->board array)
 * - Current tetromino (game->current_piece)
 * - Next tetromino preview (game->next_piece)
 * - Game metadata (score, game_over status)
 * - SDL resources (window, renderer, font)
 * - UI state (screen_state, selected_option)
 * 
 * Note: Future refactoring should separate this into distinct structs:
 * - BoardState: core board representation
 * - GameplayState: tetromino position and movement
 * - UIState: SDL resources and screen state
 * - GameMetadata: score and game status
 */
typedef struct {
    /* Core board state */
    int board[20][20];              /**< Game board grid (0 = empty, 1-7 = piece type) */
    
    /* Current piece state */
    int current_piece[4][4];        /**< Current falling tetromino (4x4 matrix) */
    int next_piece[4][4];           /**< Next tetromino preview (4x4 matrix) */
    int piece_x;                    /**< Current piece X position (column) */
    int piece_y;                    /**< Current piece Y position (row) */
    int current_type;               /**< Type of current piece (0-6 for 7 tetromino shapes) */
    int next_type;                  /**< Type of next piece (0-6 for 7 tetromino shapes) */
    
    /* Game metadata */
    int score;                      /**< Current game score */
    int game_over;                  /**< Flag: 1 if game is over, 0 otherwise */
    GameScreenState screen_state;   /**< Current screen being displayed */
    MenuOption selected_option;     /**< Currently selected menu option */
    
    /* SDL resources */
    SDL_Window *window;             /**< SDL window handle */
    SDL_Renderer *renderer;         /**< SDL renderer handle */
    TTF_Font *font;                 /**< Font for text rendering */
    int win_w;                      /**< Window width in pixels */
    int win_h;                      /**< Window height in pixels */
} GameState;

#endif // TETRIS_TYPES_H
