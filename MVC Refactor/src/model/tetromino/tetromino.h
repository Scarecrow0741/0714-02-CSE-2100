/*
 * ============================================================================
 * FILE: tetromino.h
 * PURPOSE: Tetromino class - Manages piece shapes, rotation, and position
 *          NOW IMPLEMENTS ITransformable (ISP Refactoring)
 * 
 * RESPONSIBILITY: Handle all piece-related data and transformations
 * 
 * LISKOV SUBSTITUTION PRINCIPLE (LSP) Compliance:
 *   This class maintains uniform contracts across all TetriminoShape subclasses:
 *   
 *   [✓] rotate(): Called on Tetromino, not on shape. Updates rotation state
 *       uniformly regardless of shape type (even O-piece updates state)
 *   
 *   [✓] moveBy(dx, dy): Always succeeds, position always updates uniformly.
 *       Collision detection is delegated to GameEngine (not shape-specific)
 *   
 *   [✓] getShapeMatrix(): Always returns valid 4x4 matrix, regardless of type
 *   
 *   [✓] Rotation Algorithm: All shapes use identical 90° clockwise rotation.
 *       No shape requires special rotation logic.
 *   
 *   [✓] No Shape-Specific Preconditions: Every shape can be rotated, moved,
 *       and rendered without type-checking
 * 
 * INTERFACE SEGREGATION PRINCIPLE (ISP) Improvements:
 *   This class now explicitly implements ITransformable
 *   - Code can depend on ITransformable& for movement/rotation only
 *   - GameEngine can request transformation operations specifically
 *   - Reduces coupling and increases interface clarity
 * 
 * SINGLE RESPONSIBILITY: This class is responsible ONLY for:
 *   - Managing current piece rotation state (now delegated to TetriminoShape)
 *   - Managing piece position (x, y coordinates)
 *   - Rotating pieces (uniform algorithm for all shapes)
 *   - Providing piece data for collision detection and rendering
 *   - Holding references to TetriminoShape strategy objects
 * 
 * This class DOES NOT:
 *   - Know about specific shape implementations
 *   - Store hardcoded shape definitions
 *   - Perform collision detection
 *   - Know about the Board
 *   - Know about SDL/rendering
 *   - Manage game rules or scoring
 *   - Check shape types to make behavior decisions (LSP requirement)
 * 
 * DESIGN IMPROVEMENT: Shape data is now managed by polymorphic TetriminoShape
 *   objects. To add a new shape, simply create a new TetriminoShape subclass
 *   and register it in ShapeFactory. NO CHANGES to Tetromino or GameEngine!
 * ============================================================================
 */

#ifndef TETROMINO_H
#define TETROMINO_H

#define PIECE_SIZE 4

#include "../interfaces/tetromino_shape.h"
#include "../interfaces/transformable.h"
#include <memory>

/**
 * Tetromino class - Manages piece shapes, rotation, and position
 * 
 * ISP COMPLIANCE:
 *   - Implements ITransformable for movement/rotation operations
 *   - Clients can depend on ITransformable& without needing full class knowledge
 *   - Separates transformation from shape queries
 */
class Tetromino : public ITransformable {
private:
    // Use Strategy Pattern: own TetriminoShape instances, not data
    std::unique_ptr<TetriminoShape> currentShape;
    std::unique_ptr<TetriminoShape> nextShape;
    
    // Current rotation (0-3 for 90-degree rotations)
    // POSTCONDITION: Rotation state ALWAYS updates, even for visually-symmetric shapes (LSP)
    int currentRotation;
    int nextRotation;
    
    // Position management
    int posX;
    int posY;
    
    // Cache for rotated piece (to avoid rebuilding on each access)
    mutable int rotatedPiece[PIECE_SIZE][PIECE_SIZE];
    mutable int lastRotationState;  // Track if cache is valid
    
    // Helper function to rotate a 4x4 matrix 90 degrees clockwise
    // INVARIANT: Applied uniformly to ALL shapes (LSP guarantee)
    void rotateCW(int piece[PIECE_SIZE][PIECE_SIZE]) const;
    void copyPiece(const int source[PIECE_SIZE][PIECE_SIZE], 
                   int dest[PIECE_SIZE][PIECE_SIZE]) const;
    
    // Build and cache the rotated version of the current piece
    void updateRotatedPiece() const;
    
public:
    // Constructor and Destructor
    Tetromino();
    ~Tetromino();
    
    // Initialization and spawning
    void initialize();
    void spawnNewPiece();
    
    // ITransformable Implementation
    // =================================
    /**
     * Set absolute position (ISP: ITransformable)
     */
    void setPosition(int x, int y) override;
    
    /**
     * Move by relative offset (ISP: ITransformable)
     */
    void moveBy(int dx, int dy) override;
    
    /**
     * Rotate 90 degrees clockwise (ISP: ITransformable)
     * MUST update state for all shapes (LSP contract)
     */
    void rotate() override;
    
    /**
     * Get X position (ISP: ITransformable)
     */
    int getPosX() const override { return posX; }
    
    /**
     * Get Y position (ISP: ITransformable)
     */
    int getPosY() const override { return posY; }
    
    // Additional piece data access (maintains backward compatibility)
    // ================================================================
    int getCurrentType() const;
    int getNextType() const;
    const int* getCurrentPiece() const;
    const int* getNextPiece() const;
    int getPieceAt(int x, int y) const;
    
    // Access the shape objects for polymorphic operations
    TetriminoShape* getCurrentShape() const { return currentShape.get(); }
    TetriminoShape* getNextShape() const { return nextShape.get(); }
    
    // Type-specific piece data (compatibility method, delegates to shape)
    static const int* getPieceShape(int type);
};

#endif // TETROMINO_H
