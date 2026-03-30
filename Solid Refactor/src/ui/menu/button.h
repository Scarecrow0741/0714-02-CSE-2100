#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>

/*
 * CLASS: Button
 * PURPOSE: Encapsulates button rendering and hit detection
 * RESPONSIBILITY: Single - Handle button UI state and rendering
 */
class Button {
private:
    SDL_Rect bounds;
    SDL_bool is_selected;
    SDL_bool is_hovered;
    
public:
    Button();
    
    // Setters
    void setBounds(SDL_Rect rect);
    void setSelected(SDL_bool selected);
    void setHovered(SDL_bool hovered);
    
    // Getters
    SDL_Rect* getBounds();
    SDL_bool isSelected() const;
    SDL_bool isHovered() const;
    
    // Drawing
    void draw(SDL_Renderer* renderer);
    
    // Hit detection
    SDL_bool containsPoint(int x, int y) const;
};

#endif // BUTTON_H
