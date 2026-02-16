/**
 * @file core/board/board_collision.h
 * @brief Collision detection and piece movement
 *
 * Provides collision detection, piece rotation with wall kicks,
 * and piece movement logic. Follows Single Responsibility Principle
 * by focusing exclusively on collision and movement mechanics.
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#ifndef TETRIS_BOARD_COLLISION_H
#define TETRIS_BOARD_COLLISION_H

#include "../types/tetris_types.h"

/**
 * @brief Checks if the current piece would collide
 * 
 * Detects collision with board boundaries, placed blocks, or invalid positions.
 * Used before confirming movement or rotation.
 * 
 * @param game Pointer to the GameState structure
 * 
 * @return 1 if collision detected, 0 if movement is valid
 * 
 * @details Checks:
 * - Piece blocks against board boundaries
 * - Piece blocks against already placed blocks
 * - Invalid positions (out of bounds)
 * 
 * @complexity O(16) - checks at most 16 blocks (4x4 piece)
 */
int Board_CheckCollision(const GameState *game);

/**
 * @brief Rotates the current piece with wall kick adjustment
 * 
 * Attempts to rotate the piece clockwise. If rotation causes collision,
 * tries to adjust horizontally (wall kick). If still invalid, reverts
 * to original orientation.
 * 
 * Wall kick attempts (in order):
 * - No adjustment (standard rotation)
 * - Move left 1 block
 * - Move right 1 block
 * 
 * @param game Pointer to the GameState structure
 * 
 * @return None
 * 
 * @note Modifies piece_x if wall kick is successful
 * @note Standard SRS (Super Rotation System) not implemented; simplified version
 */
void Board_RotatePiece(GameState *game);

/**
 * @brief Moves the current piece and handles locking
 * 
 * Attempts to move the piece by (dx, dy). If movement causes collision:
 * - For horizontal movement (dx != 0): reverts position (can't move left/right into blocks)
 * - For downward movement (dy > 0): locks piece in place and spawns new one
 * 
 * Locking process:
 * 1. Copy current piece to board
 * 2. Clear completed lines
 * 3. Spawn next tetromino
 * 4. Check game over condition
 * 
 * @param game Pointer to the GameState structure
 * @param dx Horizontal displacement (-1 for left, +1 for right)
 * @param dy Vertical displacement (+1 for down, 0 for no vertical change)
 * 
 * @return None
 * 
 * @complexity O(height * width) for locking; O(16) for regular movement
 * 
 * @see Board_ClearLines - called when piece is locked
 * @see Board_SpawnTetromino - called when piece is locked
 */
void Board_MovePiece(GameState *game, int dx, int dy);

#endif // TETRIS_BOARD_COLLISION_H
