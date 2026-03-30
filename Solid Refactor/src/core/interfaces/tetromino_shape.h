/*
 * ============================================================================
 * FILE: tetromino_shape.h
 * PURPOSE: Abstract base class for Tetromino shapes - Applies Strategy Pattern
 *          NOW SEGREGATED INTO FOCUSED INTERFACES (ISP Refactoring)
 * 
 * RESPONSIBILITY: Define composite interface for different Tetromino shape types
 * 
 * DESIGN PRINCIPLE: Interface Segregation Principle (ISP) + Open/Closed Principle (OCP)
 *   - OPEN for extension: New shapes can be added by creating new subclasses
 *   - CLOSED for modification: No need to modify Tetromino or GameEngine to add shapes
 *   - SEGREGATED: Now inherits from IQueryableShape and IColoredShape
 *   - BACKWARD COMPATIBLE: Existing code continues to work via TetriminoShape
 * 
 * LISKOV SUBSTITUTION PRINCIPLE (LSP):
 *   - All subclasses MUST maintain the contracts below
 *   - Clients should be able to substitute any TetriminoShape for another
 *     without changing program correctness
 *   - All shapes use the same rotation convention (90° clockwise per rotation)
 *   - All shapes return valid matrices and consistent rendering data
 * 
 * ISP IMPROVEMENT:
 *   - TetriminoShape now explicitly inherits from IQueryableShape and IColoredShape
 *   - Code can depend on IQueryableShape& for queries only
 *   - Code can depend on IColoredShape& for color only
 *   - Code can depend on TetriminoShape& for all functionality
 *   - Clients not forced to depend on unneeded methods
 * 
 * STRATEGY PATTERN: Each concrete shape (I, J, L, O, S, T, Z) is a strategy
 *   allowing the Tetromino to use different shape implementations without
 *   coupling to specific shape types.
 * ============================================================================
 */

#ifndef TETROMINO_SHAPE_H
#define TETROMINO_SHAPE_H

#define PIECE_SIZE 4

#include "queryable_shape.h"
#include "colored_shape.h"

/**
 * Composite interface for Tetromino shapes
 * Combines IQueryableShape and IColoredShape for convenience
 * 
 * ISP COMPLIANCE:
 *   - New code can depend on segregated interfaces (IQueryableShape, IColoredShape)
 *   - Backward compatibility maintained (existing code uses TetriminoShape)
 *   - Multiple inheritance allows flexible interface combinations
 * 
 * INHERITED METHODS (from IQueryableShape):
 *   - const int* getShapeMatrix() const = 0
 *   - int getTypeId() const = 0
 *   - const char* getName() const = 0
 * 
 * INHERITED METHODS (from IColoredShape):
 *   - void getColor(unsigned char&, unsigned char&, unsigned char&, unsigned char&) const = 0
 */
class TetriminoShape : public IQueryableShape, public IColoredShape {
public:
    virtual ~TetriminoShape() = default;
    
    // All methods inherited from IQueryableShape and IColoredShape
    // See those interfaces for full documentation
};

#endif // TETROMINO_SHAPE_H
