/**
 * @file core/pieces/tetris_pieces.c
 * @brief Tetromino shape data definitions
 *
 * Contains the complete definitions of all seven standard Tetris pieces.
 * These are lookup table definitions accessed during gameplay to avoid
 * runtime computation of piece rotations and configurations.
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#include "tetris_pieces.h"

/**
 * @brief Standard tetromino shapes for Tetris
 * 
 * Each of the 7 classic tetromino shapes (I, J, L, O, S, T, Z) is
 * defined as a 4x4 matrix. The elements are:
 * - 1: Block is part of this tetromino
 * - 0: Empty space in the 4x4 bounding box
 * 
 * These are the canonical orientations; rotation is handled by
 * the board/collision logic in board_collision.c
 */
const int TETRIS_SHAPES[7][4][4] = {
    /* I-piece (cyan): straight line of 4 blocks */
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    
    /* J-piece (blue): L-shape variant */
    {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    
    /* L-piece (orange): L-shape */
    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    
    /* O-piece (yellow): 2x2 square */
    {
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    
    /* S-piece (green): zigzag variant */
    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    
    /* T-piece (purple): T-shape */
    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    
    /* Z-piece (red): zigzag */
    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};
