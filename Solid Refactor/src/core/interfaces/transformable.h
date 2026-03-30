/*
 * ============================================================================
 * FILE: transformable.h
 * PURPOSE: ITransformable interface - Minimal interface for piece transformations
 *          Part of Interface Segregation Principle (ISP) refactoring
 * 
 * RESPONSIBILITY: Define interface for moving and rotating game pieces
 * 
 * DESIGN PRINCIPLE: Interface Segregation Principle (ISP)
 *   - FOCUSED: Only defines transformation operations (moveBy, rotate, setPosition)
 *   - SEGREGATED: Does NOT include query operations (getShapeMatrix, getColor)
 *   - MINIMAL: Clients only depend on the specific methods they need
 * 
 * USAGE:
 *   - GameEngine will accept ITransformable& for movement/rotation operations
 *   - This interface is implemented by Tetromino class
 *   - Renderers and collision detectors don't need this interface
 * ============================================================================
 */

#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

/**
 * Interface for objects that can be transformed (moved and rotated)
 * 
 * ISP COMPLIANCE:
 *   - This interface is focused on TRANSFORMATION only
 *   - Classes using this interface don't need to know about shape queries or colors
 *   - Clients are not forced to depend on unneeded methods
 */
class ITransformable {
public:
    virtual ~ITransformable() = default;
    
    /**
     * Set the absolute position of the piece
     * 
     * PRECONDITIONS:
     *   - x, y are valid board coordinates
     * 
     * POSTCONDITIONS:
     *   - Piece position updated to (x, y)
     *   - Return type: void (always succeeds)
     *   - No collision checking (collaboration with GameEngine)
     * 
     * INVARIANT:
     *   - Implementation must update position uniformly
     *   - All piece types must support this operation identically
     */
    virtual void setPosition(int x, int y) = 0;
    
    /**
     * Move the piece by a relative offset
     * 
     * PRECONDITIONS:
     *   - No restrictions on dx, dy values
     *   - (Collision detection is caller's responsibility)
     * 
     * POSTCONDITIONS:
     *   - Piece position incremented by (dx, dy)
     *   - Return type: void (always succeeds)
     *   - Method never fails (even if resulting position is invalid)
     * 
     * INVARIANT:
     *   - Implementation must apply offset uniformly
     *   - All piece types must support this operation identically
     */
    virtual void moveBy(int dx, int dy) = 0;
    
    /**
     * Rotate the piece 90 degrees clockwise
     * 
     * PRECONDITIONS:
     *   - Piece is in valid initialized state
     * 
     * POSTCONDITIONS:
     *   - Internal rotation state incremented (even for O-piece with visual symmetry)
     *   - Return type: void (always succeeds)
     *   - No collision checking (collaboration with GameEngine)
     *   - Rotation wraps around after 4 rotations (0->1->2->3->0)
     * 
     * LSP GUARANTEE:
     *   - All piece types (I, J, L, O, S, T, Z) rotate identically
     *   - No piece type requires special rotation handling
     *   - O-piece internal state MUST update even though shape appears identical
     *   - Enables uniform substitutability of all shape types
     * 
     * INVARIANT:
     *   - Implementation must rotate uniformly
     *   - All piece types must support this operation identically
     */
    virtual void rotate() = 0;
    
    /**
     * Get the current X position
     * 
     * PRECONDITIONS:
     *   - Object is properly initialized
     * 
     * POSTCONDITIONS:
     *   - Returns X coordinate as integer
     *   - Result is consistent across multiple calls (until position changes)
     */
    virtual int getPosX() const = 0;
    
    /**
     * Get the current Y position
     * 
     * PRECONDITIONS:
     *   - Object is properly initialized
     * 
     * POSTCONDITIONS:
     *   - Returns Y coordinate as integer
     *   - Result is consistent across multiple calls (until position changes)
     */
    virtual int getPosY() const = 0;
};

#endif // TRANSFORMABLE_H
