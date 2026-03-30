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
     * @return Pointer to a 4x4 array where 1 = filled, 0 = empty
     */
    virtual const int* getShapeMatrix() const = 0;
    
    /**
     * Get a unique identifier for this shape (0-6 for standard Tetris)
     * @return Type ID for this shape
     */
    virtual int getTypeId() const = 0;
    
    /**
     * Get the color for this shape as RGBA values
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @param a Alpha component (0-255)
     */
    virtual void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const = 0;
    
    /**
     * Get the name of this shape (for debugging/display)
     * @return String name of the shape
     */
    virtual const char* getName() const = 0;
};

#endif // TETROMINO_SHAPE_H
