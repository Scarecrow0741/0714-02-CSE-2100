/*
 * ============================================================================
 * FILE: colored_shape.h
 * PURPOSE: IColoredShape interface - Minimal interface for shape color queries
 *          Part of Interface Segregation Principle (ISP) refactoring
 * 
 * RESPONSIBILITY: Define interface for querying shape rendering color
 * 
 * DESIGN PRINCIPLE: Interface Segregation Principle (ISP)
 *   - FOCUSED: Only defines color query operation
 *   - SEGREGATED: Does NOT include shape matrix queries
 *   - SEGREGATED: Does NOT include transformation operations
 *   - MINIMAL: Renderers only depend on color interface if they need it
 * 
 * USAGE:
 *   - Renderer will accept IColoredShape& specifically for color information
 *   - This segregation allows renderers without color to exist
 *   - Pieces are colored by shape type, not by game state
 * ============================================================================
 */

#ifndef COLORED_SHAPE_H
#define COLORED_SHAPE_H

/**
 * Interface for objects that provide color information for rendering
 * 
 * ISP COMPLIANCE:
 *   - This interface is focused on COLOR RENDERING only
 *   - Classes using this interface don't need query operations (getShapeMatrix)
 *   - Classes using this interface don't need transformation operations
 *   - Clients are not forced to depend on unneeded methods
 * 
 * RATIONALE:
 *   - Future renderers might use different coloring schemes (e.g., theme-based)
 *   - By isolating color to separate interface, renderers have flexibility
 *   - Allows monochrome renderers to exist without implementing color interface
 */
class IColoredShape {
public:
    virtual ~IColoredShape() = default;
    
    /**
     * Get RGBA color values for rendering this shape
     * 
     * PRECONDITIONS:
     *   - Object is properly initialized
     *   - Output references must be valid
     * 
     * POSTCONDITIONS:
     *   - Sets r to value in range [0, 255] (Red component)
     *   - Sets g to value in range [0, 255] (Green component)
     *   - Sets b to value in range [0, 255] (Blue component)
     *   - Sets a to value in range [0, 255] (Alpha component, 255=fully opaque)
     *   - Color is deterministic for this piece type
     *   - Same color returned on repeated calls
     *   - Color does NOT vary based on game state or position
     * 
     * LSP GUARANTEE:
     *   - All shape implementations return equally valid color values
     *   - No shape requires special rendering conditions (e.g., "only show with power-up")
     *   - All shapes render with same opacity/visibility rules
     *   - All shapes' colors unique and identifiable
     * 
     * INVARIANT:
     *   - Color uniquely identifies this shape type
     *   - Different shapes have visually distinct colors
     *   - Color never changes for the lifetime of the object
     * 
     * @param r Red component output reference
     * @param g Green component output reference
     * @param b Blue component output reference
     * @param a Alpha component output reference
     */
    virtual void getColor(unsigned char& r, unsigned char& g, 
                         unsigned char& b, unsigned char& a) const = 0;
};

#endif // COLORED_SHAPE_H
