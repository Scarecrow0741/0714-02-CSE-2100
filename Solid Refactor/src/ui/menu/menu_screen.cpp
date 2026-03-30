/*
 * ============================================================================
 * FILE: menu_screen.cpp
 * PURPOSE: Implementation of MenuScreen class for main menu rendering
 * RESPONSIBILITY: Main menu UI rendering and interaction
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
    
    SDL_Color title_color = {50, 255, 150, 255};        // Bright green for title
    
    SDL_Surface* title_surface = TTF_RenderText_Solid(font, "TETRIS", title_color);
    if (title_surface) {
        SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
        SDL_Rect title_rect = {
            window_w/2 - (title_surface->w / 2),  // Centered horizontally
            (int)(window_h * 0.2),                 // 20% down from top
            title_surface->w,
            title_surface->h
        };
        SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);
        SDL_DestroyTexture(title_texture);
        SDL_FreeSurface(title_surface);
    }
}

/*
 * FUNCTION: MenuScreen::renderButtonWithText()
 * PURPOSE: Render a button with its label text
 */
void MenuScreen::renderButtonWithText(const char* text, Button* button,
                                      SDL_bool is_selected, int mouse_x, int mouse_y) {
    int window_w, window_h;
    SDL_GetRendererOutputSize(renderer, &window_w, &window_h);
    
    SDL_Color text_color = {220, 220, 220, 255};        // Off-white for normal
    SDL_Color selected_color = {255, 215, 0, 255};      // Golden yellow for selected
    
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, text,
        is_selected ? selected_color : text_color);
    
    if (text_surface) {
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        
        // Update button bounds
        float y_pos = 0;
        if (button == &play_button) {
            y_pos = 0.5;
        } else {
            y_pos = 0.65;
        }
        
        SDL_Rect button_bounds = {
            window_w/2 - (text_surface->w / 2) - 40,
            (int)(window_h * y_pos) - 15,
            text_surface->w + 80,
            text_surface->h + 30
        };
        button->setBounds(button_bounds);
        
        // Update hover state
        button->setHovered(button->containsPoint(mouse_x, mouse_y));
        button->setSelected(is_selected);
        
        // Draw button background
        button->draw(renderer);
        
        // Draw button text
        SDL_Rect text_rect = {
            window_w/2 - (text_surface->w / 2),
            (int)(window_h * y_pos),
            text_surface->w,
            text_surface->h
        };
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
        SDL_DestroyTexture(text_texture);
        SDL_FreeSurface(text_surface);
    }
}

/*
 * FUNCTION: MenuScreen::drawButtons()
 * PURPOSE: Render Play and Exit buttons with current selection state
 */
void MenuScreen::drawButtons(int mouse_x, int mouse_y, MenuOption selected) {
    renderButtonWithText("Play", &play_button, 
                        (SDL_bool)(selected == MENU_PLAY), mouse_x, mouse_y);
    renderButtonWithText("Exit", &exit_button, 
                        (SDL_bool)(selected == MENU_EXIT), mouse_x, mouse_y);
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
        return MENU_PLAY;
    }
    if (exit_button.containsPoint(x, y)) {
        return MENU_EXIT;
    }
    return MENU_PLAY;  // Default
}
