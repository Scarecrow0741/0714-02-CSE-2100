/*
 * ============================================================================
 * FILE: screen_renderer.h
 * PURPOSE: IScreenRenderer interface - Minimal interface for rendering UI screens
 *          Part of Interface Segregation Principle (ISP) refactoring
 * 
 * RESPONSIBILITY: Define interface for rendering menu and UI screens
 * 
 * DESIGN PRINCIPLE: Interface Segregation Principle (ISP)
 *   - FOCUSED: Only defines UI/screen rendering operations
 *   - SEGREGATED: Separate from IGameStateRenderer (game rendering)
 *   - SEGREGATED: Does NOT require Board or Tetromino references
 *   - MINIMAL: Menu renderers only get what they need (SDL renderer)
 * 
 * BACKGROUND:
 *   - MenuRenderer previously had to handle Board and Tetromino data
 *   - Forced MenuRenderer to accept parameters it didn't need
 *   - By segregating, MenuRenderer only receives SDL renderer
 *   - Clearer intent and reduced coupling
 * 
 * USAGE:
 *   - GameEngine will use IScreenRenderer for menu/UI screens
 *   - MenuRenderer and GameOverRenderer implement this interface
 *   - Game state renderers implement IGameStateRenderer instead
 * ============================================================================
 */

#ifndef SCREEN_RENDERER_H
#define SCREEN_RENDERER_H

#include <SDL2/SDL.h>

/**
 * Interface for rendering UI screens and menus
 * 
 * ISP COMPLIANCE:
 *   - This interface is focused on SCREEN/UI RENDERING only
 *   - Only requires SDL renderer (no game state needed)
 *   - Game state renderers don't implement this (use IGameStateRenderer instead)
 *   - Clients only depend on the specific data they need
 * 
 * RATIONALE:
 *   - Segregates UI rendering from game rendering
 *   - MenuRenderer and GameOverScreen can focus on UI-only operations
 *   - Clearer separation of concerns
 *   - Allows UI frameworks that don't know about game state
 * 
 * DESIGN PATTERN:
 *   - Implements Strategy Pattern for different UI screens
 *   - Allows adding new screens without modifying GameEngine
 */
class IScreenRenderer {
public:
    virtual ~IScreenRenderer() = default;
    
    /**
     * Render a UI/screen element
     * 
     * PRECONDITIONS:
     *   - sdlRenderer is valid and initialized
     * 
     * POSTCONDITIONS:
     *   - Screen/UI element rendered to sdlRenderer
     *   - Display ready for presentation
     *   - No internal state modified
     * 
     * INVARIANT:
     *   - Multiple render calls produce same output
     *   - No side effects beyond rendering
     * 
     * @param sdlRenderer SDL renderer to draw to
     */
    virtual void render(SDL_Renderer* sdlRenderer) = 0;
    
    /**
     * Get human-readable name of this screen renderer
     * 
     * PRECONDITIONS:
     *   - Object is properly initialized
     * 
     * POSTCONDITIONS:
     *   - Returns valid C-string (never nullptr)
     *   - String describes screen (e.g., "MainMenu", "Settings")
     *   - Used for debugging/logging only
     * 
     * @return Screen name C-string
     */
    virtual const char* getName() const = 0;
};

#endif // SCREEN_RENDERER_H
