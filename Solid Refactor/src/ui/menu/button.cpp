/*
 * ============================================================================
 * FILE: button.cpp
 * PURPOSE: Implementation of Button class for menu UI
 * RESPONSIBILITY: Button rendering and interaction state management
 * ============================================================================
 */

#include "button.h"

Button::Button() 
    : bounds({0, 0, 0, 0}), is_selected(SDL_FALSE), is_hovered(SDL_FALSE) {
}

void Button::setBounds(SDL_Rect rect) {
    bounds = rect;
}

void Button::setSelected(SDL_bool selected) {
    is_selected = selected;
}

void Button::setHovered(SDL_bool hovered) {
    is_hovered = hovered;
}

SDL_Rect* Button::getBounds() {
    return &bounds;
}

SDL_bool Button::isSelected() const {
    return is_selected;
}

SDL_bool Button::isHovered() const {
    return is_hovered;
}

/*
 * FUNCTION: Button::draw()
 * PURPOSE: Render button with styling based on state
 * 
 * Features:
 *   - Shadow effect for depth
 *   - Color changes based on selection/hover state
 *   - Border outline for visual clarity
 */
void Button::draw(SDL_Renderer* renderer) {
    // Draw shadow effect (creates depth illusion)
    SDL_Rect shadow = {bounds.x + 3, bounds.y + 3, bounds.w, bounds.h};
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);  // Dark gray shadow
    SDL_RenderFillRect(renderer, &shadow);

    // Draw button background with color based on state
    SDL_SetRenderDrawColor(renderer, 
        is_selected ? 60 : (is_hovered ? 50 : 40),      // R component
        is_selected ? 60 : (is_hovered ? 50 : 40),      // G component
        is_selected ? 80 : (is_hovered ? 70 : 60),      // B component
        255);
    SDL_RenderFillRect(renderer, &bounds);

    // Draw button border for visual definition
    SDL_SetRenderDrawColor(renderer, 
        is_selected ? 100 : (is_hovered ? 80 : 60),     // R component
        is_selected ? 100 : (is_hovered ? 80 : 60),     // G component
        is_selected ? 140 : (is_hovered ? 120 : 100),   // B component
        255);
    SDL_RenderDrawRect(renderer, &bounds);
}

/*
 * FUNCTION: Button::containsPoint()
 * PURPOSE: Check if given coordinates are within button bounds
 * PARAMETERS:
 *   x - X coordinate
 *   y - Y coordinate
 * RETURNS:
 *   SDL_TRUE if point is within button, SDL_FALSE otherwise
 */
SDL_bool Button::containsPoint(int x, int y) const {
    return (SDL_bool)(x >= bounds.x && x <= bounds.x + bounds.w &&
                      y >= bounds.y && y <= bounds.y + bounds.h);
}
