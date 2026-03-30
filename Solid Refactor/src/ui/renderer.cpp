/*
 * ============================================================================
 * FILE: renderer.cpp
 * PURPOSE: Game rendering and visual display for Tetris
 * 
 * This file handles all graphics rendering for the active gameplay:
 *   - Game board background and grid
 *   - Placed tetromino blocks with color coding
 *   - Currently falling piece
 *   - HUD elements (score, next piece preview)
 *   - Grid lines for visual guidance
 * 
 * RENDERING ORDER:
 *   1. Background (dark gray)
 *   2. Board area (green)
 *   3. Placed blocks (colored by piece type)
 *   4. Falling piece (current piece in motion)
 *   5. Grid lines (for visual clarity)
 *   6. HUD info (score box, next piece preview)
 * 
 * COLOR SCHEME (by Tetromino type):
 *   - I-piece (0): Cyan
 *   - J-piece (1): Blue
 *   - L-piece (2): Orange
 *   - O-piece (3): Yellow
 *   - S-piece (4): Green
 *   - T-piece (5): Purple
 *   - Z-piece (6): Red
 * ============================================================================
 */

#include "../include/tetris.h"
#include <bits/stdc++.h>
using namespace std;

/*
 * FUNCTION: draw_game_sdl()
 * PURPOSE: Render the entire game state to the screen
 * 
 * Renders in order:
 *   1. Dark background fill
 *   2. Game board with green background
 *   3. All placed (locked) blocks on the board
 *   4. Currently falling Tetromino piece
 *   5. Grid lines for block alignment guidance
 *   6. HUD with score text and next piece preview
 * 
 * PARAMETERS:
 *   game - Pointer to the GameState structure containing all game data
 */
void draw_game_sdl(GameState *game) {
    SDL_Renderer *r = game->renderer;
    
    // ===== BACKGROUND =====
    // Fill entire screen with dark gray (game background)
    SDL_SetRenderDrawColor(r, 5, 5, 5, 255);
    SDL_RenderClear(r);

    // ===== BOARD POSITIONING =====
    // Calculate board's screen position with padding from window edges
    int board_x = WINDOW_BORDER / 2;    // Left offset
    int board_y = WINDOW_BORDER / 4;    // Top offset

    // ===== BOARD AREA =====
    // Draw the game board background (green border area)
    SDL_Rect board_rect = { board_x - 2, board_y - 2,
                            BOARD_WIDTH * BLOCK_SIZE + 4,
                            BOARD_HEIGHT * BLOCK_SIZE + 4 };
    SDL_SetRenderDrawColor(r, 0, 80, 0, 255);  // Dark green
    SDL_RenderFillRect(r, &board_rect);

    // ===== RENDER PLACED BLOCKS =====
    // Draw all locked (non-falling) blocks on the game board
    for (int y=0; y<BOARD_HEIGHT; y++) {
        for (int x=0; x<BOARD_WIDTH; x++) {
            int cell = game->board[y][x];
            
            // Only render if cell is not empty (value > 0)
            if (cell) {
                // Calculate screen position for this block
                SDL_Rect cell_r = { 
                    board_x + x*BLOCK_SIZE, 
                    board_y + y*BLOCK_SIZE, 
                    BLOCK_SIZE - 1,  // Leave 1 pixel gap between blocks
                    BLOCK_SIZE - 1 
                };
                
                // Set color based on piece type (cell value - 1 gives piece index)
                int c = (cell - 1) % 7;
                switch (c) {
                    case 0: SDL_SetRenderDrawColor(r, 150, 220, 255, 255); break; // I - cyan
                    case 1: SDL_SetRenderDrawColor(r, 0, 128, 255, 255); break;   // J - blue
                    case 2: SDL_SetRenderDrawColor(r, 255, 165, 0, 255); break;   // L - orange
                    case 3: SDL_SetRenderDrawColor(r, 255, 220, 0, 255); break;   // O - yellow
                    case 4: SDL_SetRenderDrawColor(r, 0, 255, 0, 255); break;     // S - green
                    case 5: SDL_SetRenderDrawColor(r, 160, 32, 240, 255); break;  // T - purple
                    case 6: SDL_SetRenderDrawColor(r, 255, 80, 80, 255); break;   // Z - red
                    default: SDL_SetRenderDrawColor(r, 200,200,200,255); break;
                }
                SDL_RenderFillRect(r, &cell_r);
            }
        }
    }

    // ===== RENDER FALLING PIECE =====
    // Draw the currently active (falling) Tetromino piece
    for (int py=0; py<4; py++) {
        for (int px=0; px<4; px++) {
            // Only draw cells that are part of the piece (value = 1)
            if (game->current_piece[py][px]) {
                int bx = game->piece_x + px;    // Board X coordinate
                int by = game->piece_y + py;    // Board Y coordinate
                
                // Only draw if piece part is within visible board area
                if (by >= 0 && bx >= 0 && bx < BOARD_WIDTH && by < BOARD_HEIGHT) {
                    SDL_Rect cell_r = { 
                        board_x + bx*BLOCK_SIZE, 
                        board_y + by*BLOCK_SIZE, 
                        BLOCK_SIZE - 1, 
                        BLOCK_SIZE - 1 
                    };
                    
                    // Set color based on current piece type
                    int c = (game->current_type) % 7;
                    switch (c) {
                        case 0: SDL_SetRenderDrawColor(r, 150, 220, 255, 255); break; // I - cyan
                        case 1: SDL_SetRenderDrawColor(r, 0, 128, 255, 255); break;   // J - blue
                        case 2: SDL_SetRenderDrawColor(r, 255, 165, 0, 255); break;   // L - orange
                        case 3: SDL_SetRenderDrawColor(r, 255, 220, 0, 255); break;   // O - yellow
                        case 4: SDL_SetRenderDrawColor(r, 0, 255, 0, 255); break;     // S - green
                        case 5: SDL_SetRenderDrawColor(r, 160, 32, 240, 255); break;  // T - purple
                        case 6: SDL_SetRenderDrawColor(r, 255, 80, 80, 255); break;   // Z - red
                        default: SDL_SetRenderDrawColor(r, 200,200,200,255); break;
                    }
                    SDL_RenderFillRect(r, &cell_r);
                }
            }
        }
    }

    // ===== GRID LINES =====
    // Draw grid lines to help visualize block alignment (subtle gray lines)
    SDL_SetRenderDrawColor(r, 40, 40, 40, 255);
    
    // Vertical grid lines
    for (int gx=0; gx<=BOARD_WIDTH; gx++) {
        int x = board_x + gx*BLOCK_SIZE;
        SDL_RenderDrawLine(r, x, board_y, x, board_y + BOARD_HEIGHT*BLOCK_SIZE);
    }
    
    // Horizontal grid lines
    for (int gy=0; gy<=BOARD_HEIGHT; gy++) {
        int y = board_y + gy*BLOCK_SIZE;
        SDL_RenderDrawLine(r, board_x, y, board_x + BOARD_WIDTH*BLOCK_SIZE, y);
    }

    // ===== HUD (Heads-Up Display) =====
    // Position HUD to the right of the game board
    int hud_x = board_x + BOARD_WIDTH*BLOCK_SIZE + 20;  // 20 pixel gap from board
    int hud_y = board_y;

    // ===== SCORE BOX =====
    SDL_Rect score_box = { hud_x, hud_y, HUD_WIDTH - 40, 60 };
    SDL_SetRenderDrawColor(r, 30, 30, 30, 255);  // Dark gray box
    SDL_RenderFillRect(r, &score_box);

    // Render score text if font is available
    if (game->font) {
        char score_text[32];
        sprintf(score_text, "SCORE: %d", game->score);
        
        SDL_Color text_color = {255, 255, 255, 255};  // White text
        SDL_Surface* text_surface = TTF_RenderText_Solid(game->font, score_text, text_color);
        
        if (text_surface) {
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(r, text_surface);
            if (text_texture) {
                SDL_Rect text_rect = { 
                    hud_x + 10,                    // 10 pixel padding left
                    hud_y + 20,                    // 20 pixel padding top
                    text_surface->w, 
                    text_surface->h 
                };
                SDL_RenderCopy(r, text_texture, NULL, &text_rect);
                SDL_DestroyTexture(text_texture);
            }
            SDL_FreeSurface(text_surface);
        }
    }

    // ===== NEXT PIECE PREVIEW =====
    // Area where the next upcoming piece will be displayed
    int preview_x = hud_x;
    int preview_y = hud_y + 80;  // Below the score box
    
    SDL_Rect preview_box = { 
        preview_x, 
        preview_y, 
        NEXT_PREVIEW_SIZE * BLOCK_SIZE + 6,  // 4x4 grid for piece display
        NEXT_PREVIEW_SIZE * BLOCK_SIZE + 6 
    };
    SDL_SetRenderDrawColor(r, 30, 30, 30, 255);  // Dark gray box
    SDL_RenderFillRect(r, &preview_box);

    // Draw the next piece in 4x4 preview area
    for (int py=0; py<4; py++) {
        for (int px=0; px<4; px++) {
            // Only draw cells that are part of the next piece
            if (game->next_piece[py][px]) {
                SDL_Rect cell_r = { 
                    preview_x + px*BLOCK_SIZE + 1,  // 1 pixel offset for borders
                    preview_y + py*BLOCK_SIZE + 1, 
                    BLOCK_SIZE - 2, 
                    BLOCK_SIZE - 2 
                };
                SDL_SetRenderDrawColor(r, 200,200,200,255);  // Light gray for preview
                SDL_RenderFillRect(r, &cell_r);
            }
        }
    }

    // ===== CONSOLE OUTPUT =====
    // Print score to console for debugging/progress tracking
    printf("\rSCORE: %d  ", game->score);
    fflush(stdout);

    // ===== PRESENT FRAME =====
    // Update the display with all rendered graphics
    SDL_RenderPresent(r);
}

