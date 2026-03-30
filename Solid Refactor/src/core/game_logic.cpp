/*
 * ============================================================================
 * FILE: game_logic.cpp
 * PURPOSE: Core game mechanics and logic for Tetris
 * 
 * This file contains all the game logic functions that handle:
 *   - Game board initialization
 *   - Tetromino (piece) spawning and management
 *   - Collision detection
 *   - Piece rotation with wall kick
 *   - Piece movement (left, right, down)
 *   - Line clearing and score calculation
 * 
 * All pieces are represented as 4x4 matrices where 1 indicates a filled cell
 * and 0 indicates an empty cell. The TETROMINOS array stores all 7 Tetris
 * piece definitions (I, J, L, O, S, T, Z shapes).
 * ============================================================================
 */

#include "../include/tetris.h"
#include <bits/stdc++.h>
using namespace std;

// Array storing all 7 Tetromino shapes (I, J, L, O, S, T, Z)
// Each shape is represented as a 4x4 grid where 1 = filled, 0 = empty
const int TETROMINOS[7][4][4] = {
    // I-piece (cyan line)
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    // J-piece (blue L-shape)
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // L-piece (orange L-shape)
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // O-piece (yellow square)
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // S-piece (green zigzag)
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // T-piece (purple T-shape)
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // Z-piece (red zigzag)
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
};

/*
 * FUNCTION: init_game()
 * PURPOSE: Initialize the game state and board
 * 
 * Sets up a new game by:
 *   - Clearing the game board (filling all cells with EMPTY_CELL)
 *   - Resetting the score to 0
 *   - Setting game_over flag to 0 (game is not over)
 *   - Positioning the first piece at the top-center of the board
 *   - Seeding the random number generator for piece selection
 *   - Spawning the first Tetromino
 * 
 * PARAMETERS:
 *   game - Pointer to the GameState structure to initialize
 */
void init_game(GameState *game) {
    // Clear the board by filling all cells with empty value
    for (int y=0; y<BOARD_HEIGHT; y++)
        for (int x=0; x<BOARD_WIDTH; x++)
            game->board[y][x] = EMPTY_CELL;

    // Reset game state variables
    game->score = 0;
    game->game_over = 0;
    
    // Position the piece at the top-center of the board
    game->piece_x = (BOARD_WIDTH/2) - 2;
    game->piece_y = 0;

    // Seed random number generator with current time
    srand((unsigned)time(NULL));
    
    // Generate the first next piece type
    game->next_type = rand() % 7;
    
    // Spawn the first active Tetromino
    spawn_tetromino(game);
}

/*
 * FUNCTION: spawn_tetromino()
 * PURPOSE: Create a new falling Tetromino piece
 * 
 * This function:
 *   - Moves the "next" piece to the current active piece
 *   - Generates a new random "next" piece for preview
 *   - Copies the piece data from the TETROMINOS array (4x4 grid)
 *   - Positions the piece at the top-center of the board
 *   - Checks for immediate collision (game over condition)
 * 
 * PARAMETERS:
 *   game - Pointer to the GameState structure
 */
void spawn_tetromino(GameState *game) {
    // Move next piece to become the current piece
    game->current_type = game->next_type;
    
    // Generate a new random piece for the next_piece preview
    game->next_type = rand() % 7;

    // Copy the Tetromino shape data from the template array to the game state
    for (int y=0; y<4; y++)
        for (int x=0; x<4; x++) {
            game->current_piece[y][x] = TETROMINOS[game->current_type][y][x];
            game->next_piece[y][x] = TETROMINOS[game->next_type][y][x];
        }

    // Reset position to top-center
    game->piece_x = (BOARD_WIDTH/2) - 2;
    game->piece_y = 0;

    // Check if the new piece immediately collides (game over)
    if (check_collision(game)) {
        game->game_over = 1;
    }
}

/*
 * FUNCTION: check_collision()
 * PURPOSE: Detect if the current piece collides with the board or walls
 * 
 * Checks if the active piece:
 *   - Goes beyond the left or right boundaries of the board
 *   - Goes beyond the bottom boundary of the board
 *   - Overlaps with already placed pieces on the board
 * 
 * PARAMETERS:
 *   game - Pointer to the GameState structure (const - does not modify)
 * 
 * RETURNS:
 *   1 (true) if collision detected, 0 (false) if no collision
 */
int check_collision(const GameState *game) {
    // Iterate through each cell of the current 4x4 piece
    for (int y=0; y<4; y++) {
        for (int x=0; x<4; x++) {
            // Only check cells that are part of the piece (value = 1)
            if (game->current_piece[y][x]) {
                // Calculate the piece's position on the game board
                int board_x = game->piece_x + x;
                int board_y = game->piece_y + y;

                // Check boundary conditions and collisions with placed blocks
                if (board_x < 0 || board_x >= BOARD_WIDTH ||
                    board_y >= BOARD_HEIGHT ||
                    (board_y >= 0 && game->board[board_y][board_x])) {
                    return 1;  // Collision detected
                }
            }
        }
    }
    return 0;  // No collision
}

/*
 * FUNCTION: rotate_piece()
 * PURPOSE: Rotate the current piece 90 degrees clockwise
 * 
 * Process:
 *   1. Create a rotated version of the piece using matrix rotation
 *   2. Save the original piece configuration
 *   3. Apply the rotation
 *   4. Check for collision after rotation
 *   5. If collision occurs, attempt wall-kick (shift piece left/right)
 *   6. If still colliding, revert to original orientation
 * 
 * PARAMETERS:
 *   game - Pointer to the GameState structure
 */
void rotate_piece(GameState *game) {
    // Create a rotated version by transposing and reversing columns
    // Formula: rotated[x][3-y] = original[y][x]
    int rotated[4][4] = {0};
    for (int y=0; y<4; y++)
        for (int x=0; x<4; x++)
            rotated[x][3 - y] = game->current_piece[y][x];

    // Save the original piece configuration in case rotation fails
    int original[4][4];
    for (int y=0; y<4; y++)
        for (int x=0; x<4; x++) {
            original[y][x] = game->current_piece[y][x];
            game->current_piece[y][x] = rotated[y][x];
        }

    // Check if the rotated piece collides with anything
    if (check_collision(game)) {
        // Try wall-kick: shift piece 1 unit to the left
        game->piece_x -= 1;
        if (check_collision(game)) {
            // Wall-kick failed on left, try shifting 2 units to the right
            game->piece_x += 2;
            if (check_collision(game)) {
                // Wall-kick failed, revert to original position and rotation
                game->piece_x -= 1;
                for (int y=0; y<4; y++)
                    for (int x=0; x<4; x++)
                        game->current_piece[y][x] = original[y][x];
            }
        }
    }
}

/*
 * FUNCTION: move_piece()
 * PURPOSE: Move the current piece horizontally or vertically
 * 
 * Handles:
 *   - Horizontal movement (left/right) by changing piece_x
 *   - Vertical movement (down) by changing piece_y
 *   - Collision detection and undo on collision
 *   - Locking piece to board when it hits bottom
 *   - Spawning next piece and clearing lines
 * 
 * PARAMETERS:
 *   game - Pointer to the GameState structure
 *   dx - Horizontal movement delta (-1 left, +1 right, 0 no change)
 *   dy - Vertical movement delta (positive moves down)
 */
void move_piece(GameState *game, int dx, int dy) {
    // Apply the movement
    game->piece_x += dx;
    game->piece_y += dy;

    // Check if the new position causes a collision
    if (check_collision(game)) {
        // Undo the movement
        game->piece_x -= dx;
        game->piece_y -= dy;

        // If moving downward caused collision, piece has landed
        if (dy > 0) {
            // Lock the piece to the board by storing its type value in board cells
            for (int y=0; y<4; y++) {
                for (int x=0; x<4; x++) {
                    if (game->current_piece[y][x]) {
                        int board_x = game->piece_x + x;
                        int board_y = game->piece_y + y;
                        // Only place on board if within bounds
                        if (board_y >= 0 && board_x >= 0 && board_x < BOARD_WIDTH && board_y < BOARD_HEIGHT) {
                            game->board[board_y][board_x] = game->current_type + 1;
                        }
                    }
                }
            }
            // Check for and clear any complete lines
            clear_lines(game);
            // Spawn the next Tetromino
            spawn_tetromino(game);
        }
    }
}

/*
 * FUNCTION: clear_lines()
 * PURPOSE: Check for and remove complete rows from the board
 * 
 * Process:
 *   1. Scan from bottom to top for complete lines
 *   2. When a complete line is found, remove it
 *   3. Drop all lines above down by one row
 *   4. Create a new empty line at the top
 *   5. Calculate score based on number of lines cleared
 *    - Formula: lines_cleared^2 * 100
 * 
 * PARAMETERS:
 *   game - Pointer to the GameState structure
 */
void clear_lines(GameState *game) {
    int lines_cleared = 0;

    // Scan board from bottom to top
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        // Check if current line is complete (no empty cells)
        int line_complete = 1;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!game->board[y][x]) {
                // Found an empty cell, line is not complete
                line_complete = 0;
                break;
            }
        }
        
        if (line_complete) {
            // Line is complete, increment counter and remove it
            lines_cleared++;
            
            // Drop all lines above down by one row
            for (int y2 = y; y2 > 0; y2--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    game->board[y2][x] = game->board[y2 - 1][x];
                }
            }
            
            // Clear the top line
            for (int x = 0; x < BOARD_WIDTH; x++) {
                game->board[0][x] = EMPTY_CELL;
            }
            
            // Increment y to re-check the same row (now contains shifted pieces)
            y++;
        }
    }

    // Update score based on lines cleared
    if (lines_cleared > 0) {
        // Score multiplier: clearing more lines gives exponentially higher scores
        game->score += lines_cleared * lines_cleared * 100;
    }
}

