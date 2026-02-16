#include "tetris.h"
#include <stdio.h>

void draw_game_sdl(GameState *game) {
    SDL_Renderer *r = game->renderer;
    // background
    SDL_SetRenderDrawColor(r, 5, 5, 5, 255);
    SDL_RenderClear(r);

    // compute board origin
    int board_x = WINDOW_BORDER / 2;
    int board_y = WINDOW_BORDER / 4;

    // draw grid background
    SDL_Rect board_rect = { board_x - 2, board_y - 2,
                            BOARD_WIDTH * BLOCK_SIZE + 4,
                            BOARD_HEIGHT * BLOCK_SIZE + 4 };
    SDL_SetRenderDrawColor(r, 0, 80, 0, 255);
    SDL_RenderFillRect(r, &board_rect);

    // draw placed blocks
    for (int y=0; y<BOARD_HEIGHT; y++) {
        for (int x=0; x<BOARD_WIDTH; x++) {
            int cell = game->board[y][x];
            if (cell) {
                SDL_Rect cell_r = { board_x + x*BLOCK_SIZE, board_y + y*BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1 };
                // color per piece type (simple palette)
                int c = (cell - 1) % 7;
                switch (c) {
                    case 0: SDL_SetRenderDrawColor(r, 150, 220, 255, 255); break; // I - cyan-ish
                    case 1: SDL_SetRenderDrawColor(r, 0, 128, 255, 255); break; // J
                    case 2: SDL_SetRenderDrawColor(r, 255, 165, 0, 255); break; // L
                    case 3: SDL_SetRenderDrawColor(r, 255, 220, 0, 255); break; // O
                    case 4: SDL_SetRenderDrawColor(r, 0, 255, 0, 255); break; // S
                    case 5: SDL_SetRenderDrawColor(r, 160, 32, 240, 255); break; // T
                    case 6: SDL_SetRenderDrawColor(r, 255, 80, 80, 255); break; // Z
                    default: SDL_SetRenderDrawColor(r, 200,200,200,255); break;
                }
                SDL_RenderFillRect(r, &cell_r);
            }
        }
    }

    // draw current moving piece
    for (int py=0; py<4; py++) {
        for (int px=0; px<4; px++) {
            if (game->current_piece[py][px]) {
                int bx = game->piece_x + px;
                int by = game->piece_y + py;
                if (by >= 0 && bx >= 0 && bx < BOARD_WIDTH && by < BOARD_HEIGHT) {
                    SDL_Rect cell_r = { board_x + bx*BLOCK_SIZE, board_y + by*BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1 };
                    int c = (game->current_type) % 7;
                    switch (c) {
                        case 0: SDL_SetRenderDrawColor(r, 150, 220, 255, 255); break;
                        case 1: SDL_SetRenderDrawColor(r, 0, 128, 255, 255); break;
                        case 2: SDL_SetRenderDrawColor(r, 255, 165, 0, 255); break;
                        case 3: SDL_SetRenderDrawColor(r, 255, 220, 0, 255); break;
                        case 4: SDL_SetRenderDrawColor(r, 0, 255, 0, 255); break;
                        case 5: SDL_SetRenderDrawColor(r, 160, 32, 240, 255); break;
                        case 6: SDL_SetRenderDrawColor(r, 255, 80, 80, 255); break;
                        default: SDL_SetRenderDrawColor(r, 200,200,200,255); break;
                    }
                    SDL_RenderFillRect(r, &cell_r);
                }
            }
        }
    }

    // draw grid lines (optional thin lines)
    SDL_SetRenderDrawColor(r, 40, 40, 40, 255);
    for (int gx=0; gx<=BOARD_WIDTH; gx++) {
        int x = board_x + gx*BLOCK_SIZE;
        SDL_RenderDrawLine(r, x, board_y, x, board_y + BOARD_HEIGHT*BLOCK_SIZE);
    }
    for (int gy=0; gy<=BOARD_HEIGHT; gy++) {
        int y = board_y + gy*BLOCK_SIZE;
        SDL_RenderDrawLine(r, board_x, y, board_x + BOARD_WIDTH*BLOCK_SIZE, y);
    }

    // HUD: score and next piece
    int hud_x = board_x + BOARD_WIDTH*BLOCK_SIZE + 20;
    int hud_y = board_y;

    // Score box
    SDL_Rect score_box = { hud_x, hud_y, HUD_WIDTH - 40, 60 };
    SDL_SetRenderDrawColor(r, 30, 30, 30, 255);
    SDL_RenderFillRect(r, &score_box);

    // Render score text
    if (game->font) {
        char score_text[32];
        sprintf(score_text, "SCORE: %d", game->score);
        SDL_Color text_color = {255, 255, 255, 255};  // White color
        SDL_Surface* text_surface = TTF_RenderText_Solid(game->font, score_text, text_color);
        if (text_surface) {
            SDL_Texture* text_texture = SDL_CreateTextureFromSurface(r, text_surface);
            if (text_texture) {
                SDL_Rect text_rect = { hud_x + 10, hud_y + 20, text_surface->w, text_surface->h };
                SDL_RenderCopy(r, text_texture, NULL, &text_rect);
                SDL_DestroyTexture(text_texture);
            }
            SDL_FreeSurface(text_surface);
        }
    }

    // Next piece preview area
    int preview_x = hud_x;
    int preview_y = hud_y + 80;
    SDL_Rect preview_box = { preview_x, preview_y, NEXT_PREVIEW_SIZE * BLOCK_SIZE + 6, NEXT_PREVIEW_SIZE * BLOCK_SIZE + 6 };
    SDL_SetRenderDrawColor(r, 30, 30, 30, 255);
    SDL_RenderFillRect(r, &preview_box);

    // Draw next piece
    for (int py=0; py<4; py++) {
        for (int px=0; px<4; px++) {
            if (game->next_piece[py][px]) {
                SDL_Rect cell_r = { preview_x + px*BLOCK_SIZE + 1, preview_y + py*BLOCK_SIZE + 1, BLOCK_SIZE - 2, BLOCK_SIZE - 2 };
                SDL_SetRenderDrawColor(r, 200,200,200,255);
                SDL_RenderFillRect(r, &cell_r);
            }
        }
    }
    printf("\rSCORE: %d  ", game->score);
    fflush(stdout);

    SDL_RenderPresent(r);
}