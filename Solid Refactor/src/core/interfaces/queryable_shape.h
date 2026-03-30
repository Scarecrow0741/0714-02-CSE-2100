/*
 * ============================================================================
 * FILE: queryable_shape.h
 * PURPOSE: IQueryableShape interface - Minimal interface for shape data queries
 *          Part of Interface Segregation Principle (ISP) refactoring
 * 
 * RESPONSIBILITY: Define interface for querying shape data (matrix, metadata)
 * 
 * DESIGN PRINCIPLE: Interface Segregation Principle (ISP)
 *   - FOCUSED: Only defines data query operations
 *   - SEGREGATED: Does NOT include transformation operations (moveBy, rotate)
 *   - SEGREGATED: Does NOT include color information (use separately)
 *   - MINIMAL: Clients only depend on the specific methods they need
 * 
 * USAGE:
 *   - Board and GameEngine will accept IQueryableShape& for collision detection
 *   - Renderer will query shape data through this interface
 *   - Movement/rotation operations use separate ITransformable interface
 * 
 * LSP COMPLIANCE:
 *   - All 7 standard Tetromino shapes implement this interface identically
 *   - No shape requires special preconditions for method calls
 *   - All shapes return consistent valid data
 * ============================================================================
 */

#ifndef QUERYABLE_SHAPE_H
#define QUERYABLE_SHAPE_H

#define PIECE_SIZE 4

/**
 * Interface for objects that provide queryable shape data
 * 
 * ISP COMPLIANCE:
 *   - This interface is focused on DATA QUERIES only
 *   - Classes using this interface don't need transformation operations
 *   - Classes using this interface don't need color information
 *   - Clients are not forced to depend on unneeded methods
 */
class IQueryableShape {
public:
    virtual ~IQueryableShape() = default;
    
    /**
     * Get the 4x4 matrix representing this shape
     * 
     * PRECONDITIONS:
     *   - Object is in valid initialized state
     * 
     * POSTCONDITIONS:
     *   - Returns pointer to 4x4 array (16 integers)
     *   - Array contains only 0 (empty) or 1 (filled) values
     *   - Pointer is valid for object's lifetime
     *   - Same matrix returned on repeated calls (for same shape)
     *   - All returned matrices represent valid connected Tetromino pieces
     * 
     * LSP GUARANTEE:
     *   - All shape implementations return equally valid matrices
     *   - No shape requires special handling to retrieve matrix
     *   - Matrix format identical for all shapes
     * 
     * @return Pointer to 4x4 int array [row][col]
     */
    virtual const int* getShapeMatrix() const = 0;
    
    /**
     * Get unique identifier for this shape type
     * 
     * PRECONDITIONS:
     *   - Object is properly initialized
     * 
     * POSTCONDITIONS:
     *   - Returns non-negative integer [0, N) where N = number of shape types
     *   - Result is deterministic (same ID for same shape type)
     *   - ID uniquely identifies the shape (0=I, 1=O, 2=T, 3=S, 4=Z, 5=J, 6=L)
     *   - ID is used for debugging/logging only (never for behavior decisions - LSP)
     * 
     * INVARIANT:
     *   - Type ID never changes for the lifetime of the object
     *   - Two objects with different type IDs are different shape types
     * 
     * @return Shape type identifier
     */
    virtual int getTypeId() const = 0;
    
    /**
     * Get human-readable name of this shape
     * 
     * PRECONDITIONS:
     *   - Object is properly initialized
     * 
     * POSTCONDITIONS:
     *   - Returns valid C-string (never nullptr)
     *   - String is null-terminated
     *   - String describes shape type (e.g., "I-Piece", "O-Piece")
     *   - Same string returned on repeated calls
     *   - Used for debugging/logging only (never for behavior decisions - LSP)
     * 
     * INVARIANT:
     *   - Name never changes for the lifetime of the object
     *   - Different shapes have different names
     * 
     * @return C-string describing shape type
     */
    virtual const char* getName() const = 0;
};

#endif // QUERYABLE_SHAPE_H
