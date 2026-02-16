/**
 * @file core/board/board_collision.c
 * @brief Implementation of collision detection and movement logic
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#include "board_collision.h"
#include "board_state.h"
#include "../types/tetris_constants.h"
#include <string.h>

int Board_CheckCollision(const GameState *game) {
    /* Check all blocks of the current piece */
    for (int y = 0; y < TETRIS_PIECE_SIZE; y++) {
        for (int x = 0; x < TETRIS_PIECE_SIZE; x++) {
            if (game->current_piece[y][x]) {
                /* Calculate board position */
                int board_x = game->piece_x + x;
                int board_y = game->piece_y + y;

                /* Check bounds */
                if (board_x < 0 || board_x >= TETRIS_BOARD_WIDTH ||
                    board_y >= TETRIS_BOARD_HEIGHT) {
                    return 1; /* Collision detected */
                }

                /* Check collision with placed blocks (above board is OK during spawn) */
                if (board_y >= 0 && game->board[board_y][board_x]) {
                    return 1; /* Collision detected */
                }
            }
        }
    }
    return 0; /* No collision */
}

void Board_RotatePiece(GameState *game) {
    int rotated[TETRIS_PIECE_SIZE][TETRIS_PIECE_SIZE] = {0};
    
    /* Rotate clockwise: rotated[x][3-y] = original[y][x] */
    for (int y = 0; y < TETRIS_PIECE_SIZE; y++) {
        for (int x = 0; x < TETRIS_PIECE_SIZE; x++) {
            rotated[x][TETRIS_PIECE_SIZE - 1 - y] = game->current_piece[y][x];
        }
    }

    /* Save original piece state for revert */
    int original[TETRIS_PIECE_SIZE][TETRIS_PIECE_SIZE];
    for (int y = 0; y < TETRIS_PIECE_SIZE; y++) {
        for (int x = 0; x < TETRIS_PIECE_SIZE; x++) {
            original[y][x] = game->current_piece[y][x];
            game->current_piece[y][x] = rotated[y][x];
        }
    }

    /* Check if rotation is valid */
    if (Board_CheckCollision(game)) {
        /* Try wall kick - adjust left */
        game->piece_x -= 1;
        if (Board_CheckCollision(game)) {
            /* Try wall kick - adjust right */
            game->piece_x += 2;
            if (Board_CheckCollision(game)) {
                /* Wall kick failed, revert to original rotation and position */
                game->piece_x -= 1;
                for (int y = 0; y < TETRIS_PIECE_SIZE; y++) {
                    for (int x = 0; x < TETRIS_PIECE_SIZE; x++) {
                        game->current_piece[y][x] = original[y][x];
                    }
                }
            }
        }
    }
}

void Board_MovePiece(GameState *game, int dx, int dy) {
    /* Update position */
    game->piece_x += dx;
    game->piece_y += dy;

    /* Check for collision at new position */
    if (Board_CheckCollision(game)) {
        /* Revert movement */
        game->piece_x -= dx;
        game->piece_y -= dy;

        /* If moving down (dy > 0), piece has landed - lock it in place */
        if (dy > 0) {
            /* Lock piece to board */
            for (int y = 0; y < TETRIS_PIECE_SIZE; y++) {
                for (int x = 0; x < TETRIS_PIECE_SIZE; x++) {
                    if (game->current_piece[y][x]) {
                        int board_x = game->piece_x + x;
                        int board_y = game->piece_y + y;
                        
                        /* Only place if on valid board position */
                        if (board_y >= 0 && board_x >= 0 && 
                            board_x < TETRIS_BOARD_WIDTH && 
                            board_y < TETRIS_BOARD_HEIGHT) {
                            game->board[board_y][board_x] = game->current_type + 1;
                        }
                    }
                }
            }
            
            /* Clear completed lines */
            Board_ClearLines(game);
            
            /* Spawn next piece */
            Board_SpawnTetromino(game);
        }
    }
}
