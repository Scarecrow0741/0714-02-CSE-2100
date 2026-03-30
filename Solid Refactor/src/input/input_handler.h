/*
 * ============================================================================
 * FILE: input_handler.h
 * PURPOSE: InputHandler class - Translates SDL events to game commands
 * 
 * RESPONSIBILITY: Handle all input processing
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

class InputHandler {
private:
    SDL_Event event;
    bool shouldQuit;
    
public:
    InputHandler();
    ~InputHandler();
    
    // Poll and process events
    void pollEvents();
    
    // Query command state
    GameCommand getCommand();
    
    // Query quit state
    bool isQuitRequested() const { return shouldQuit; }
    void quit() { shouldQuit = true; }
    
    // Mouse/menu interaction
    void getMousePosition(int& x, int& y) const;
    bool isMouseButtonPressed() const;
};

#endif // INPUT_HANDLER_H
