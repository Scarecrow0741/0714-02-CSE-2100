/*
 * ============================================================================
 * FILE: menu_screen.cpp
 * PURPOSE: Implementation of MenuScreen class for main menu rendering
 * RESPONSIBILITY: Main menu UI rendering and interaction
 * 
 * VIEW LAYER: UI screen implementation
 * ============================================================================
 */

#include "menu_screen.h"

MenuScreen::MenuScreen(SDL_Renderer* sdl_renderer, TTF_Font* sdl_font)
    : renderer(sdl_renderer), font(sdl_font) {
}

/*
 * FUNCTION: MenuScreen::drawBackground()
 * PURPOSE: Render background with grid pattern
 */
void MenuScreen::drawBackground() {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);

    // Clear screen with dark blue background
    SDL_SetRenderDrawColor(renderer, 0, 0, 20, 255);
    SDL_RenderClear(renderer);

    // Draw grid pattern background (aesthetic element)
    SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
    for(int x = 0; x < window_w; x += 40) {
        SDL_RenderDrawLine(renderer, x, 0, x, window_h);  // Vertical lines
    }
    for(int y = 0; y < window_h; y += 40) {
        SDL_RenderDrawLine(renderer, 0, y, window_w, y);  // Horizontal lines
    }
}

/*
 * FUNCTION: MenuScreen::drawTitle()
 * PURPOSE: Render the TETRIS title
 */
void MenuScreen::drawTitle() {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
    
    // Simple title box (text rendering disabled)
    SDL_Rect title_box = {
        window_w/2 - 150,
        (int)(window_h * 0.2),
        300,
        50
    };
    SDL_SetRenderDrawColor(renderer, 50, 255, 150, 255);
    SDL_RenderDrawRect(renderer, &title_box);
}

/*
 * FUNCTION: MenuScreen::renderButtonWithText()
 * PURPOSE: Render a button with its label text
 */
void MenuScreen::renderButtonWithText(const char* text, Button* button,
                                      bool is_selected, int mouse_x, int mouse_y) {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
    
    // Simple button box (text rendering disabled)
    float y_pos = (button == &play_button) ? 0.5 : 0.65;
    
    SDL_Rect button_bounds = {
        window_w/2 - 100,
        (int)(window_h * y_pos) - 15,
        200,
        50
    };
    button->setBounds(button_bounds);
    
    button->setHovered(button->containsPoint(mouse_x, mouse_y) ? SDL_TRUE : SDL_FALSE);
    button->setSelected(is_selected ? SDL_TRUE : SDL_FALSE);
    
    // Draw button
    button->draw(renderer);
}

/*
 * FUNCTION: MenuScreen::drawButtons()
 * PURPOSE: Render Play and Exit buttons with current selection state
 */
void MenuScreen::drawButtons(int mouse_x, int mouse_y, MenuOption selected) {
    renderButtonWithText("Play", &play_button, 
                        (selected == MenuOption::MENU_PLAY), mouse_x, mouse_y);
    renderButtonWithText("Exit", &exit_button, 
                        (selected == MenuOption::MENU_EXIT), mouse_x, mouse_y);
}

/*
 * FUNCTION: MenuScreen::draw()
 * PURPOSE: Render the complete main menu
 */
void MenuScreen::draw(MenuOption selected) {
    // Get current mouse position for hover detection
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    
    drawBackground();
    drawTitle();
    drawButtons(mouse_x, mouse_y, selected);
    
    // Display the rendered frame
    SDL_RenderPresent(renderer);
}

/*
 * FUNCTION: MenuScreen::handleClick()
 * PURPOSE: Determine which button was clicked (if any)
 * RETURNS: MenuOption corresponding to clicked button, or MENU_PLAY if none
 */
MenuOption MenuScreen::handleClick(int x, int y) {
    if (play_button.containsPoint(x, y)) {
        return MenuOption::MENU_PLAY;
    }
    if (exit_button.containsPoint(x, y)) {
        return MenuOption::MENU_EXIT;
    }
    return MenuOption::MENU_PLAY;  // Default
}
