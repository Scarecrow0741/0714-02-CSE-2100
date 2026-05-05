/*
 * ============================================================================
 * FILE: input_handler.h
 * PURPOSE: InputHandler class - Translates SDL events to game commands
 *          NOW IMPLEMENTS IInputProvider (ISP Refactoring)
 * 
 * RESPONSIBILITY: Handle all input processing
 * 
 * INTERFACE SEGREGATION PRINCIPLE (ISP) Improvement:
 *   This class now explicitly implements IInputProvider
 *   - GameEngine depends on IInputProvider interface (not concrete InputHandler)
 *   - Decouples GameEngine from SDL implementation details
 *   - Allows testing with mock input providers
 *   - Allows alternative input sources (network, gamepad, AI, etc.)
 * 
 * SINGLE RESPONSIBILITY: This class is responsible ONLY for:
 *   - Polling SDL events
 *   - Translating raw keystrokes to game commands
 *   - Tracking which commands are active
 *   - Converting mouse clicks to menu actions
 * 
 * This class DOES NOT:
 *   - Execute game logic
 *   - Know about the Board or Tetromino
 *   - Handle rendering
 * ============================================================================
 */

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "../../model/interfaces/input_provider.h"

/**
 * InputHandler class - Translates SDL events to game commands
 * 
 * ISP COMPLIANCE:
 *   - Implements IInputProvider interface
 *   - GameEngine depends on IInputProvider interface
 *   - Hides SDL implementation details from GameEngine
 *   - Allows swapping input implementations without GameEngine changes
 */
class InputHandler : public IInputProvider {
private:
    SDL_Event event;
    bool shouldQuit;
    
public:
    InputHandler();
    ~InputHandler();
    
    // IInputProvider Implementation
    // =============================
    
    /**
     * Poll and process events (IInputProvider)
     */
    void pollEvents() override;
    
    /**
     * Query command state (IInputProvider)
     */
    GameCommand getCommand() override;
    
    /**
     * Query quit state (IInputProvider)
     */
    bool isQuitRequested() const override { return shouldQuit; }
    
    /**
     * Request quit (IInputProvider)
     */
    void quit() override { shouldQuit = true; }
    
    /**
     * Get mouse position (IInputProvider)
     */
    void getMousePosition(int& x, int& y) const override;
    
    /**
     * Check mouse button (IInputProvider)
     */
    bool isMouseButtonPressed() const override;
};

#endif // INPUT_HANDLER_H
