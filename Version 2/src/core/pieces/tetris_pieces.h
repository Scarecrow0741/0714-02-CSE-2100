/**
 * @file core/pieces/tetris_pieces.h
 * @brief Tetromino shape definitions and piece-related declarations
 *
 * Provides access to the seven standard Tetris pieces (tetrominoes) and
 * piece-related utility functions. The piece definitions are stored in
 * a lookup table for O(1) access time during gameplay.
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#ifndef TETRIS_PIECES_H
#define TETRIS_PIECES_H

/**
 * @brief Array of all seven tetromino shapes
 * 
 * Standard Tetris piece definitions:
 * - 0: I-piece (cyan) - four in a row
 * - 1: J-piece (blue) - L-shape variant
 * - 2: L-piece (orange) - L-shape
 * - 3: O-piece (yellow) - 2x2 square
 * - 4: S-piece (green) - zigzag variant
 * - 5: T-piece (purple) - T-shape
 * - 6: Z-piece (red) - zigzag
 * 
 * Each piece is represented as a 4x4 matrix where:
 * - 1 indicates a filled block
 * - 0 indicates empty space
 * 
 * @see TETRIS_NUM_SHAPES
 * @see TETRIS_PIECE_SIZE
 */
extern const int TETRIS_SHAPES[7][4][4];

#endif // TETRIS_PIECES_H
