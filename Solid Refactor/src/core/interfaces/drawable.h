/*
 * ============================================================================
 * FILE: drawable.h
 * PURPOSE: Abstract interface for drawable game objects - Prepares for extension
 * 
 * RESPONSIBILITY: Define interface for objects that can be rendered
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP)
 *   - OPEN for extension: New drawable types (ghost pieces, particles, effects)
 *     can be added without modifying Renderer
 *   - CLOSED for modification: Renderer only needs to know about Drawable interface
 * 
 * ADAPTER PATTERN: This interface adapts different game objects to a common
 *   rendering interface, allowing future extension with ghost pieces, particle
 *   effects, visual indicators, etc. without modifying the Renderer.
 * ============================================================================
 */

#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <SDL2/SDL.h>

class Drawable {
public:
    virtual ~Drawable() = default;
    
    /**
     * Render this drawable object to the given SDL renderer
     * 
     * @param renderer SDL renderer to draw to
     * @param blockSize Size of a single block in pixels
     * @param offsetX Pixel offset for the board's X coordinate
     * @param offsetY Pixel offset for the board's Y coordinate
     */
    virtual void draw(SDL_Renderer* renderer, int blockSize, int offsetX, int offsetY) = 0;
    
    /**
     * Get the type identifier for this drawable (for filtering/prioritization)
     * @return Type ID (0=standard block, 1=ghost, 2=particle, etc.)
     */
    virtual int getDrawType() const = 0;
};

#endif // DRAWABLE_H
