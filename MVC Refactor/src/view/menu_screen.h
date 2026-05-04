#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "button.h"

/*
 * Menu option enum - UI state for main menu
 */
enum class MenuOption {
    MENU_PLAY,
    MENU_EXIT
};

/*
 * CLASS: MenuScreen
 * PURPOSE: Manages main menu screen rendering and interaction
 * RESPONSIBILITY: Single - Render and manage main menu UI
 * 
 * VIEW LAYER: UI screen implementation, no Model dependencies
 */
class MenuScreen {
private:
    Button play_button;
    Button exit_button;
    TTF_Font* font;
    SDL_Renderer* renderer;
    
    // Helper methods for rendering
    void drawBackground();
    void drawTitle();
    void drawButtons(int mouse_x, int mouse_y, MenuOption selected);
    void renderButtonWithText(const char* text, Button* button, 
                             bool is_selected, int mouse_x, int mouse_y);
    
public:
    MenuScreen(SDL_Renderer* sdl_renderer, TTF_Font* sdl_font);
    
    // Main rendering function
    void draw(MenuOption selected);
    
    // Input handling
    MenuOption handleClick(int x, int y);
};

#endif // MENU_SCREEN_H
