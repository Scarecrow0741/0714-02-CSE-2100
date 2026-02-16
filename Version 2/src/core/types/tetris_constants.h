/**
 * @file core/types/tetris_constants.h
 * @brief Game constants and configuration parameters for Tetris engine
 *
 * Centralized location for all game constants including board dimensions,
 * visual parameters, and gameplay tuning values. Following the chess engine
 * refactoring model, constants use MODULE_UPPER_SNAKE_CASE naming convention.
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#ifndef TETRIS_CONSTANTS_H
#define TETRIS_CONSTANTS_H

/* ========== Board Dimensions ========== */

/** @brief Width of the game board in blocks */
#define TETRIS_BOARD_WIDTH 20

/** @brief Height of the game board in blocks */
#define TETRIS_BOARD_HEIGHT 20

/** @brief Value representing an empty cell on the board */
#define TETRIS_EMPTY_CELL 0

/* ========== Visual Parameters ========== */

/** @brief Window border size in pixels */
#define TETRIS_WINDOW_BORDER 50

/** @brief Size of each tetromino block in pixels */
#define TETRIS_BLOCK_SIZE 20

/** @brief Size of next piece preview area in blocks */
#define TETRIS_NEXT_PREVIEW_SIZE 4

/** @brief Width of the right HUD panel in pixels */
#define TETRIS_HUD_WIDTH 200

/* ========== Tetromino Configuration ========== */

/** @brief Number of tetromino types (I, J, L, O, S, T, Z) */
#define TETRIS_NUM_SHAPES 7

/** @brief Size of tetromino rotation matrix (standard 4x4) */
#define TETRIS_PIECE_SIZE 4

/* ========== Gameplay Parameters ========== */

/** @brief Default piece drop interval in milliseconds */
#define TETRIS_DROP_INTERVAL_MS 500

/** @brief Frame rate cap for menu and game over screens */
#define TETRIS_MENU_FPS_CAP_MS 16

/** @brief Frame rate cap during gameplay */
#define TETRIS_GAMEPLAY_FPS_CAP_MS 8

/** @brief Score multiplier for clearing lines (lines_cleared^2 * this_value) */
#define TETRIS_LINE_CLEAR_SCORE_MULTIPLIER 100

#endif // TETRIS_CONSTANTS_H
