/**
 * @file ui/sdl/sdl_renderer.c
 * @brief SDL2 rendering implementation for Tetris gameplay
 *
 * Handles all visual rendering of the game board, pieces, and HUD.
 * Uses lookup-based color mapping for piece types to ensure O(1) color selection.
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#include "sdl_renderer.h"
#include "../../core/types/tetris_constants.h"
#include <stdio.h>

/**
 * @brief Color palette for tetromino pieces
 * 
 * Maps piece type (0-6) to RGBA colors:
 * - 0: I-piece (cyan)
 * - 1: J-piece (blue)
 * - 2: L-piece (orange)
 * - 3: O-piece (yellow)
 * - 4: S-piece (green)
 * - 5: T-piece (purple)
 * - 6: Z-piece (red)
 */
static const SDL_Color TETRIS_PIECE_COLORS[7] = {
    {150, 220, 255, 255},  /* I - cyan */
    {0, 128, 255, 255},    /* J - blue */
    {255, 165, 0, 255},    /* L - orange */
    {255, 220, 0, 255},    /* O - yellow */
    {0, 255, 0, 255},      /* S - green */
    {160, 32, 240, 255},   /* T - purple */
    {255, 80, 80, 255}     /* Z - red */
};

/**
 * @brief Gets the color for a given piece type
 * 
 * @param piece_type Piece type (0-6)
 * @return SDL_Color for rendering
 */
static SDL_Color getPieceColor(int piece_type) {
    int color_index = piece_type % 7;
    return TETRIS_PIECE_COLORS[color_index];
}

void SDLRenderer_DrawGameplay(GameState *game) {
    SDL_Renderer *renderer = game->renderer;
    
    /* Clear background */
    SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
    SDL_RenderClear(renderer);

    /* Calculate board origin position */
    int board_x = TETRIS_WINDOW_BORDER / 2;
    int board_y = TETRIS_WINDOW_BORDER / 4;

    /* Draw board background border */
    SDL_Rect board_rect = {
        board_x - 2,
        board_y - 2,
        TETRIS_BOARD_WIDTH * TETRIS_BLOCK_SIZE + 4,
        TETRIS_BOARD_HEIGHT * TETRIS_BLOCK_SIZE + 4
    };
    SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);
    SDL_RenderFillRect(renderer, &board_rect);

    /* Draw all placed blocks on board */
    for (int y = 0; y < TETRIS_BOARD_HEIGHT; y++) {
        for (int x = 0; x < TETRIS_BOARD_WIDTH; x++) {
            if (game->board[y][x]) {
                SDL_Rect cell_rect = {
                    board_x + x * TETRIS_BLOCK_SIZE,
                    board_y + y * TETRIS_BLOCK_SIZE,
                    TETRIS_BLOCK_SIZE - 1,
                    TETRIS_BLOCK_SIZE - 1
                };
                SDL_Color color = getPieceColor(game->board[y][x] - 1);
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(renderer, &cell_rect);
            }
        }
    }

    /* Draw current falling piece */
    for (int py = 0; py < TETRIS_PIECE_SIZE; py++) {
        for (int px = 0; px < TETRIS_PIECE_SIZE; px++) {
            if (game->current_piece[py][px]) {
                int screen_x = game->piece_x + px;
                int screen_y = game->piece_y + py;
                
                /* Only draw if on visible board area */
                if (screen_y >= 0 && screen_x >= 0 && 
                    screen_x < TETRIS_BOARD_WIDTH && 
                    screen_y < TETRIS_BOARD_HEIGHT) {
                    SDL_Rect cell_rect = {
                        board_x + screen_x * TETRIS_BLOCK_SIZE,
                        board_y + screen_y * TETRIS_BLOCK_SIZE,
                        TETRIS_BLOCK_SIZE - 1,
                        TETRIS_BLOCK_SIZE - 1
                    };
                    SDL_Color color = getPieceColor(game->current_type);
                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                    SDL_RenderFillRect(renderer, &cell_rect);
                }
            }
        }
    }

    /* Draw grid lines for visual clarity */
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    
    /* Vertical grid lines */
    for (int gx = 0; gx <= TETRIS_BOARD_WIDTH; gx++) {
        int x = board_x + gx * TETRIS_BLOCK_SIZE;
        SDL_RenderDrawLine(renderer, x, board_y, 
                          x, board_y + TETRIS_BOARD_HEIGHT * TETRIS_BLOCK_SIZE);
    }
    
    /* Horizontal grid lines */
    for (int gy = 0; gy <= TETRIS_BOARD_HEIGHT; gy++) {
        int y = board_y + gy * TETRIS_BLOCK_SIZE;
        SDL_RenderDrawLine(renderer, board_x, y, 
                          board_x + TETRIS_BOARD_WIDTH * TETRIS_BLOCK_SIZE, y);
    }

    /* Draw HUD panel */
    int hud_x = board_x + TETRIS_BOARD_WIDTH * TETRIS_BLOCK_SIZE + 20;
    int hud_y = board_y;

    /* Score display background */
    SDL_Rect score_box = {hud_x, hud_y, TETRIS_HUD_WIDTH - 40, 60};
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &score_box);

    /* Score text */
    if (game->font) {
        char score_text[32];
        sprintf(score_text, "SCORE: %d", game->score);
        SDL_Color text_color = {255, 255, 255, 255};
        SDL_Surface *text_surface = TTF_RenderText_Solid(game->font, score_text, text_color);
        
        if (text_surface) {
            SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
            if (text_texture) {
                SDL_Rect text_rect = {hud_x + 10, hud_y + 20, text_surface->w, text_surface->h};
                SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
                SDL_DestroyTexture(text_texture);
            }
            SDL_FreeSurface(text_surface);
        }
    }

    /* Next piece preview section */
    int preview_x = hud_x;
    int preview_y = hud_y + 80;
    SDL_Rect preview_box = {
        preview_x,
        preview_y,
        TETRIS_NEXT_PREVIEW_SIZE * TETRIS_BLOCK_SIZE + 6,
        TETRIS_NEXT_PREVIEW_SIZE * TETRIS_BLOCK_SIZE + 6
    };
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &preview_box);

    /* Draw next piece preview */
    for (int py = 0; py < TETRIS_PIECE_SIZE; py++) {
        for (int px = 0; px < TETRIS_PIECE_SIZE; px++) {
            if (game->next_piece[py][px]) {
                SDL_Rect cell_rect = {
                    preview_x + px * TETRIS_BLOCK_SIZE + 1,
                    preview_y + py * TETRIS_BLOCK_SIZE + 1,
                    TETRIS_BLOCK_SIZE - 2,
                    TETRIS_BLOCK_SIZE - 2
                };
                SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                SDL_RenderFillRect(renderer, &cell_rect);
            }
        }
    }

    /* Debug output to console */
    printf("\rSCORE: %d  ", game->score);
    fflush(stdout);

    /* Present frame to screen */
    SDL_RenderPresent(renderer);
}
