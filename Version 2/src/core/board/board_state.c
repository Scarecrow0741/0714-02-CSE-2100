/**
 * @file core/board/board_state.c
 * @brief Implementation of board state management
 *
 * Handles board initialization, tetromino spawning, and line clearing logic.
 * Core gameplay mechanics that don't depend on collision detection.
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#include "board_state.h"
#include "board_collision.h"
#include "../pieces/tetris_pieces.h"
#include "../types/tetris_constants.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

void Board_InitializeGame(GameState *game) {
    /* Clear game board */
    for (int y = 0; y < TETRIS_BOARD_HEIGHT; y++) {
        for (int x = 0; x < TETRIS_BOARD_WIDTH; x++) {
            game->board[y][x] = TETRIS_EMPTY_CELL;
        }
    }

    /* Reset game metadata */
    game->score = 0;
    game->game_over = 0;
    game->piece_x = (TETRIS_BOARD_WIDTH / 2) - 2;
    game->piece_y = 0;

    /* Seed random number generator and spawn initial pieces */
    srand((unsigned)time(NULL));
    game->next_type = rand() % TETRIS_NUM_SHAPES;
    Board_SpawnTetromino(game);
}

void Board_SpawnTetromino(GameState *game) {
    /* Move next piece to current */
    game->current_type = game->next_type;
    game->next_type = rand() % TETRIS_NUM_SHAPES;

    /* Copy piece matrices from shape definitions */
    for (int y = 0; y < TETRIS_PIECE_SIZE; y++) {
        for (int x = 0; x < TETRIS_PIECE_SIZE; x++) {
            game->current_piece[y][x] = TETRIS_SHAPES[game->current_type][y][x];
            game->next_piece[y][x] = TETRIS_SHAPES[game->next_type][y][x];
        }
    }

    /* Position at top-center of board */
    game->piece_x = (TETRIS_BOARD_WIDTH / 2) - 2;
    game->piece_y = 0;

    /* Check if game ends immediately (no room for new piece) */
    if (Board_CheckCollision(game)) {
        game->game_over = 1;
    }
}

int Board_ClearLines(GameState *game) {
    int lines_cleared = 0;

    /* Scan board from bottom to top */
    for (int y = TETRIS_BOARD_HEIGHT - 1; y >= 0; y--) {
        int line_complete = 1;
        
        /* Check if this line is completely filled */
        for (int x = 0; x < TETRIS_BOARD_WIDTH; x++) {
            if (!game->board[y][x]) {
                line_complete = 0;
                break;
            }
        }
        
        if (line_complete) {
            lines_cleared++;
            
            /* Shift all lines above down by one */
            for (int y2 = y; y2 > 0; y2--) {
                for (int x = 0; x < TETRIS_BOARD_WIDTH; x++) {
                    game->board[y2][x] = game->board[y2 - 1][x];
                }
            }
            
            /* Clear top line */
            for (int x = 0; x < TETRIS_BOARD_WIDTH; x++) {
                game->board[0][x] = TETRIS_EMPTY_CELL;
            }
            
            /* Stay at same y to check this line again (it now has new content) */
            y++;
        }
    }

    /* Update score: lines_cleared^2 * 100 */
    if (lines_cleared > 0) {
        game->score += lines_cleared * lines_cleared * TETRIS_LINE_CLEAR_SCORE_MULTIPLIER;
    }

    return lines_cleared;
}
