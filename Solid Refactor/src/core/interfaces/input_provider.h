/*
 * ============================================================================
 * FILE: input_provider.h
 * PURPOSE: IInputProvider interface - Minimal interface for input operations
 *          Part of Interface Segregation Principle (ISP) refactoring
 * 
 * RESPONSIBILITY: Define interface for input event handling
 * 
 * DESIGN PRINCIPLE: Interface Segregation Principle (ISP)
 *   - FOCUSED: Only defines input-related operations
 *   - SEGREGATED: Does NOT expose SDL_Event directly
 *   - SEGREGATED: Does NOT expose internal input state
 *   - MINIMAL: GameEngine only depends on methods it actually uses
 * 
 * BACKGROUND:
 *   - GameEngine currently depends on concrete InputHandler class
 *   - This forces inheritance/dependency on SDL implementation details
 *   - ISP suggests GameEngine should depend on small focused interface
 *   - Multiple input providers could implement this (keyboard, gamepad, network, etc.)
 * 
 * USAGE:
 *   - GameEngine will accept IInputProvider& for all input operations
 *   - Decouples GameEngine from SDL details
 *   - Allows multiple input implementation strategies
 * ============================================================================
 */

#ifndef INPUT_PROVIDER_H
#define INPUT_PROVIDER_H

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
     * Check if quit has been requested (e.g., window close button)
     * 
     * PRECONDITIONS:
     *   - pollEvents() may or may not have been called
     * 
     * POSTCONDITIONS:
     *   - Returns true if quit requested, false otherwise
     *   - Result persists until quit() is called or reset
     * 
     * INVARIANT:
     *   - Once quit is requested, always returns true until reset
     *   - Multiple calls return consistent result
     * 
     * @return True if quit requested, false otherwise
     */
    virtual bool isQuitRequested() const = 0;
    
    /**
     * Explicitly request quit/shutdown
     * 
     * PRECONDITIONS:
     *   - Object is properly initialized
     * 
     * POSTCONDITIONS:
     *   - isQuitRequested() will return true after this call
     *   - Signals the game to begin shutdown sequence
     */
    virtual void quit() = 0;
    
    /**
     * Get current mouse position on screen
     * 
     * PRECONDITIONS:
     *   - x and y are valid output references
     * 
     * POSTCONDITIONS:
     *   - Sets x to current mouse X coordinate (in pixels)
     *   - Sets y to current mouse Y coordinate (in pixels)
     *   - Coordinates are screen space (not board space)
     * 
     * INVARIANT:
     *   - Coordinates are always valid after call
     *   - No negative values unless outside window
     */
    virtual void getMousePosition(int& x, int& y) const = 0;
    
    /**
     * Check if mouse button is currently pressed
     * 
     * PRECONDITIONS:
     *   - None (can be called anytime)
     * 
     * POSTCONDITIONS:
     *   - Returns true if primary mouse button pressed, false otherwise
     *   - Reflects current state at time of call
     * 
     * INVARIANT:
     *   - Multiple calls return consistent result until mouse released
     * 
     * @return True if mouse button pressed, false otherwise
     */
    virtual bool isMouseButtonPressed() const = 0;
};

#endif // INPUT_PROVIDER_H
