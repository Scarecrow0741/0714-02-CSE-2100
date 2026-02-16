/**
 * @file core/board/board_state.h
 * @brief Board state management and initialization
 *
 * Provides functions for initializing game state, spawning tetrominoes,
 * and tracking line clears. Follows Single Responsibility Principle by
 * handling only board state lifecycle.
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#ifndef TETRIS_BOARD_STATE_H
#define TETRIS_BOARD_STATE_H

#include "../types/tetris_types.h"

/**
 * @brief Initializes a new game state
 * 
 * Sets up the game board, piece positions, and initial game data.
 * Called when starting a new game or after game over.
 * 
 * @param game Pointer to the GameState structure to initialize
 * 
 * @return None
 * 
 * @note Clears the board, resets score to 0, and spawns the first tetromino
 * @warning Caller must ensure game pointer is valid (non-null)
 */
void Board_InitializeGame(GameState *game);

/**
 * @brief Spawns a new tetromino at the top of the board
 * 
 * Moves the next piece to current, generates a new next piece,
 * and positions the current piece at spawn location (center-top).
 * Checks for immediate collision (game over condition).
 * 
 * @param game Pointer to the GameState structure
 * 
 * @return None
 * 
 * @note Sets game->game_over to 1 if the new piece collides immediately
 * @see Board_CheckCollision
 */
void Board_SpawnTetromino(GameState *game);

/**
 * @brief Clears completed lines from the board and updates score
 * 
 * Scans the board for full lines (no empty cells),
 * removes them, and shifts remaining blocks down.
 * Updates score based on number of lines cleared.
 * 
 * Scoring formula: lines_cleared^2 * 100
 * - 1 line: 100 points
 * - 2 lines: 400 points
 * - 3 lines: 900 points
 * - 4 lines (Tetris): 1600 points
 * 
 * @param game Pointer to the GameState structure
 * 
 * @return Number of lines that were cleared
 * 
 * @complexity O(height * width) - scans entire board
 */
int Board_ClearLines(GameState *game);

#endif // TETRIS_BOARD_STATE_H
