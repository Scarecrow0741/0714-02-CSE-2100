/*
 * ============================================================================
 * FILE: iinput_provider.h
 * PURPOSE: IInputProvider interface - Minimal interface for input operations
 *          CONTROLLER LAYER - Moved from core/interfaces/
 *
 * RESPONSIBILITY: Define interface for input event handling
 * 
 * DESIGN PRINCIPLE: Interface Segregation Principle (ISP)
 *   - FOCUSED: Only defines input-related operations
 *   - SEGREGATED: Does NOT expose SDL_Event directly
 *   - SEGREGATED: Does NOT expose internal input state
 *   - MINIMAL: GameEngine only depends on methods it actually uses
 * 
 * DEPENDENCY INVERSION:
 *   - GameEngine depends on IInputProvider interface (not concrete InputHandler)
 *   - Decouples GameEngine from SDL implementation details
 *   - Allows testing with mock input providers
 *   - Allows alternative input sources (keyboard, gamepad, network, AI, etc.)
 * 
 * USAGE:
 *   - GameEngine accepts IInputProvider* for all input operations
 *   - Decouples GameEngine from SDL details
 *   - Allows multiple input implementation strategies
 *
 * CONTROLLER LAYER COMPONENT: Input abstraction interface
 * ============================================================================
 */

#ifndef IINPUT_PROVIDER_H
#define IINPUT_PROVIDER_H

/**
 * Enumeration of game commands that can be executed
 * Represents abstracted input instead of SDL-specific events
 */
enum class GameCommand {
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    ROTATE,
    HARD_DROP,
    QUIT,
    MENU_PLAY,
    MENU_EXIT
};

/**
 * Interface for input event processing
 * 
 * ISP COMPLIANCE:
 *   - This interface defines only the input methods that GameEngine needs
 *   - GameEngine doesn't need to know about SDL or raw events
 *   - Other classes can implement this interface with different input sources
 *   - Clients are not forced to depend on SDL-specific structures
 * 
 * RATIONALE:
 *   - Separates input abstraction from input implementation
 *   - GameEngine depends on abstraction (IInputProvider), not implementation (InputHandler)
 *   - Dependency Inversion Principle complement
 */
class IInputProvider {
public:
    virtual ~IInputProvider() = default;
    
    /**
     * Poll and process all pending input events
     * 
     * PRECONDITIONS:
     *   - Input provider is properly initialized
     * 
     * POSTCONDITIONS:
     *   - All pending events processed
     *   - Internal state updated based on events
     *   - Ready to return commands via getCommand()
     * 
     * INVARIANT:
     *   - Method is safe to call repeatedly
     *   - No internal state corrupted by multiple calls
     *   - Events are consumed (won't be seen in next pollEvents call)
     */
    virtual void pollEvents() = 0;
    
    /**
     * Get the current game command from recent events
     * 
     * PRECONDITIONS:
     *   - pollEvents() has been called since last command retrieval
     * 
     * POSTCONDITIONS:
     *   - Returns GameCommand enum value
     *   - Returns NONE if no command is active
     *   - Multiple calls return same command until events repoll
     * 
     * INVARIANT:
     *   - Returns valid GameCommand enum always
     *   - Never returns nullptr or invalid values
     * 
     * @return Current game command (NONE if no input)
     */
    virtual GameCommand getCommand() = 0;
    
    /**
     * Query if quit was requested
     * 
     * POSTCONDITIONS:
     *   - Returns true if quit signal received (Q key, window close, etc.)
     *   - Returns false otherwise
     * 
     * @return true if quit requested, false otherwise
     */
    virtual bool isQuitRequested() const = 0;
    
    /**
     * Signal a quit request
     * 
     * POSTCONDITIONS:
     *   - isQuitRequested() will return true after this call
     *   - Can be called externally or by event handlers
     */
    virtual void quit() = 0;
    
    /**
     * Get current mouse position
     * 
     * POSTCONDITIONS:
     *   - x and y set to current mouse coordinates
     *   - Values relative to game window
     * 
     * @param x Output: mouse X coordinate
     * @param y Output: mouse Y coordinate
     */
    virtual void getMousePosition(int& x, int& y) const = 0;
    
    /**
     * Check if mouse button is currently pressed
     * 
     * POSTCONDITIONS:
     *   - Returns true if left mouse button pressed
     *   - Returns false otherwise
     * 
     * @return true if mouse button pressed, false otherwise
     */
    virtual bool isMouseButtonPressed() const = 0;
};

#endif // IINPUT_PROVIDER_H
