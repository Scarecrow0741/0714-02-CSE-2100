/*
 * ============================================================================
 * FILE: tetromino_shape.h
 * PURPOSE: Abstract base class for Tetromino shapes - Applies Strategy Pattern
 * 
 * RESPONSIBILITY: Define interface for different Tetromino shape types
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP)
 *   - OPEN for extension: New shapes can be added by creating new subclasses
 *   - CLOSED for modification: No need to modify Tetromino or GameEngine to add shapes
 * 
 * LISKOV SUBSTITUTION PRINCIPLE (LSP):
 *   - All subclasses MUST maintain the contracts below
 *   - Clients should be able to substitute any TetriminoShape for another
 *     without changing program correctness
 *   - All shapes use the same rotation convention (90° clockwise per rotation)
 *   - All shapes return valid matrices and consistent rendering data
 * 
 * STRATEGY PATTERN: Each concrete shape (I, J, L, O, S, T, Z) is a strategy
 *   allowing the Tetromino to use different shape implementations without
 *   coupling to specific shape types.
 * ============================================================================
 */

#ifndef TETROMINO_SHAPE_H
#define TETROMINO_SHAPE_H

#define PIECE_SIZE 4

class TetriminoShape {
public:
    virtual ~TetriminoShape() = default;
    
    /**
     * Get the 4x4 matrix representing this shape in its base rotation state
     * 
     * PRECONDITIONS:
     *   - Object is properly constructed
     * 
     * POSTCONDITIONS:
     *   - Returns valid pointer to 4x4 array (never nullptr)
     *   - Array contains only values 0 or 1
     *   - Array layout is always [row][col] with row=0 at top
     *   - Result is deterministic (always same for same object)
     *   - Result is immutable during object lifetime
     * 
     * LSP GUARANTEE:
     *   - Every shape's matrix is a valid Tetromino piece (connected and 4 cells)
     *   - Can be rotated consistently using standard 90° clockwise rotation
     *   - No shape-specific preconditions for rotation
     * 
     * @return Pointer to a 4x4 array where 1 = filled, 0 = empty
     */
    virtual const int* getShapeMatrix() const = 0;
    
    /**
     * Get a unique identifier for this shape (0-6 for standard Tetris)
     * 
     * PRECONDITIONS:
     *   - Object is properly constructed
     * 
     * POSTCONDITIONS:
     *   - Returns a non-negative integer in range [0, N) where N = num_shape_types
     *   - Result is deterministic (always same for same object)
     *   - Two shapes with different IDs are different types
     *   - ID uniquely identifies the shape type
     * 
     * LSP GUARANTEE:
     *   - Type ID is used for display/debugging purposes only
     *   - GameEngine does NOT perform type-based behavior changes
     *   - ID is never used for collision detection or movement decisions
     * 
     * @return Type ID for this shape
     */
    virtual int getTypeId() const = 0;
    
    /**
     * Get the color for this shape as RGBA values
     * 
     * PRECONDITIONS:
     *   - Object is properly constructed
     *   - Output parameters are valid references
     * 
     * POSTCONDITIONS:
     *   - Sets r, g, b, a to values in range [0, 255]
     *   - Color is consistent across multiple calls
     *   - Color uniquely identifies this shape type
     * 
     * LSP GUARANTEE:
     *   - Color is determined by shape type, not game state
     *   - No shape requires special rendering conditions (e.g., "power-up required to display")
     *   - All shapes render with same visibility/opacity rules
     * 
     * @param r Red component (0-255) - output reference
     * @param g Green component (0-255) - output reference
     * @param b Blue component (0-255) - output reference
     * @param a Alpha component (0-255) - output reference
     */
    virtual void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const = 0;
    
    /**
     * Get the human-readable name of this shape (for debugging/display)
     * 
     * PRECONDITIONS:
     *   - Object is properly constructed
     * 
     * POSTCONDITIONS:
     *   - Returns valid C-string (never nullptr)
     *   - String is null-terminated
     *   - String describes the shape (e.g., "I-Piece", "O-Piece")
     *   - Name is consistent across multiple calls
     * 
     * LSP GUARANTEE:
     *   - Name is for debugging only; never used for behavior decisions
     * 
     * @return String name of the shape
     */
    virtual const char* getName() const = 0;
};

#endif // TETROMINO_SHAPE_H
