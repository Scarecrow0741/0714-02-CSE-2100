/*
 * ============================================================================
 * FILE: input_handler.cpp
 * PURPOSE: Implementation of InputHandler class
 *
 * CONTROLLER LAYER COMPONENT: Input handler implementation
 * ============================================================================
 */

#include "input_handler.h"
#include <cstring>

InputHandler::InputHandler() : shouldQuit(false) {
    memset(&event, 0, sizeof(SDL_Event));
}

InputHandler::~InputHandler() {
    // No dynamic memory
}

void InputHandler::pollEvents() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                shouldQuit = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_q) {
                    shouldQuit = true;
                }
                break;
            default:
                break;
        }
    }
}

GameCommand InputHandler::getCommand() {
    // Check current keyboard state
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    
    // Check for movement commands
    if (keystates[SDL_SCANCODE_A] || keystates[SDL_SCANCODE_LEFT]) {
        return GameCommand::MOVE_LEFT;
    }
    if (keystates[SDL_SCANCODE_D] || keystates[SDL_SCANCODE_RIGHT]) {
        return GameCommand::MOVE_RIGHT;
    }
    if (keystates[SDL_SCANCODE_S] || keystates[SDL_SCANCODE_DOWN]) {
        return GameCommand::MOVE_DOWN;
    }
    if (keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_UP]) {
        return GameCommand::ROTATE;
    }
    if (keystates[SDL_SCANCODE_SPACE]) {
        return GameCommand::HARD_DROP;
    }
    
    return GameCommand::NONE;
}

void InputHandler::getMousePosition(int& x, int& y) const {
    SDL_GetMouseState(&x, &y);
}

bool InputHandler::isMouseButtonPressed() const {
    Uint32 buttons = SDL_GetMouseState(NULL, NULL);
    return (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
}
