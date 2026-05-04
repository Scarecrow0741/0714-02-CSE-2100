/*
 * ============================================================================
 * FILE: renderer.cpp
 * PURPOSE: Implementation of Renderer class (OCP-COMPLIANT)
 * 
 * DESIGN IMPROVEMENTS:
 *   - Colors are now obtained from TetriminoShape objects (polymorphic)
 *   - Removes hardcoded color switch statement
 *   - Easy to extend: new shapes automatically get their colors
 *   - Reads from Model layer (const references only)
 * ============================================================================
 */

#include "renderer.h"
#include "../model/shape_factory.h"
#include <cstdio>

Renderer::Renderer(SDL_Renderer* sdlRenderer, TTF_Font* sdlFont)
    : renderer(sdlRenderer), font(sdlFont), boardX(WINDOW_BORDER / 2), boardY(WINDOW_BORDER / 4) {
}

Renderer::~Renderer() {
    // No cleanup needed (renderer and font are managed elsewhere)
}

void Renderer::setColorForPieceType(int type) {
    // NEW: Use polymorphic shape to get color (OCP: no hardcoded switch statement!)
    auto shape = ShapeFactory::createShape(type);
    if (shape) {
        unsigned char r, g, b, a;
        shape->getColor(r, g, b, a);
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    } else {
        // Safe fallback
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    }
}

void Renderer::drawBackground() {
    // Fill entire screen with dark gray (game background)
    SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
    SDL_RenderClear(renderer);
    
    // Draw the game board background (green border area)
    SDL_Rect boardRect = {
        boardX - 2, boardY - 2,
        BOARD_WIDTH * BLOCK_SIZE + 4,
        BOARD_HEIGHT * BLOCK_SIZE + 4
    };
    SDL_SetRenderDrawColor(renderer, 0, 80, 0, 255);  // Dark green
    SDL_RenderFillRect(renderer, &boardRect);
}

void Renderer::drawBoard(const Board& board) {
    // Draw all locked (non-falling) blocks on the game board
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            int cell = board.getCell(x, y);
            
            // Only render if cell is not empty (value > 0)
            if (cell > 0) {
                SDL_Rect cellRect = {
                    boardX + x * BLOCK_SIZE,
                    boardY + y * BLOCK_SIZE,
                    BLOCK_SIZE - 1,
                    BLOCK_SIZE - 1
                };
                
                setColorForPieceType(cell - 1);
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }
    }
}

void Renderer::drawFallingPiece(const Tetromino& tetromino, const Board& board) {
    // Draw the currently active (falling) Tetromino piece
    int posX = tetromino.getPosX();
    int posY = tetromino.getPosY();
    int type = tetromino.getCurrentType();
    
    for (int py = 0; py < 4; py++) {
        for (int px = 0; px < 4; px++) {
            // Only draw cells that are part of the piece (value = 1)
            if (tetromino.getPieceAt(px, py)) {
                int bx = posX + px;    // Board X coordinate
                int by = posY + py;    // Board Y coordinate
                
                // Only draw if piece part is within visible board area
                if (by >= 0 && bx >= 0 && bx < BOARD_WIDTH && by < BOARD_HEIGHT) {
                    SDL_Rect cellRect = {
                        boardX + bx * BLOCK_SIZE,
                        boardY + by * BLOCK_SIZE,
                        BLOCK_SIZE - 1,
                        BLOCK_SIZE - 1
                    };
                    
                    setColorForPieceType(type);
                    SDL_RenderFillRect(renderer, &cellRect);
                }
            }
        }
    }
}

void Renderer::drawGridLines() {
    // Draw grid lines to help visualize block alignment (subtle gray lines)
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    
    // Vertical grid lines
    for (int gx = 0; gx <= BOARD_WIDTH; gx++) {
        int x = boardX + gx * BLOCK_SIZE;
        SDL_RenderDrawLine(renderer, x, boardY, x, boardY + BOARD_HEIGHT * BLOCK_SIZE);
    }
    
    // Horizontal grid lines
    for (int gy = 0; gy <= BOARD_HEIGHT; gy++) {
        int y = boardY + gy * BLOCK_SIZE;
        SDL_RenderDrawLine(renderer, boardX, y, boardX + BOARD_WIDTH * BLOCK_SIZE, y);
    }
}

void Renderer::drawHUD(int score, const Tetromino& tetromino) {
    // Position HUD to the right of the game board
    int hudX = boardX + BOARD_WIDTH * BLOCK_SIZE + 20;
    int hudY = boardY;
    
    // ===== SCORE BOX =====
    SDL_Rect scoreBox = { hudX, hudY, HUD_WIDTH - 40, 60 };
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);  // Dark gray box
    SDL_RenderFillRect(renderer, &scoreBox);
    
    // Note: Text rendering disabled (SDL2_ttf library not available)
    // Just draw the score box for now
    
    // ===== NEXT PIECE PREVIEW =====
    int previewX = hudX;
    int previewY = hudY + 100;
    
    // Draw "NEXT:" label (text rendering disabled)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect labelBox = { previewX, previewY, 60, 20 };
    SDL_RenderDrawRect(renderer, &labelBox);
    
    // Draw next piece preview box
    SDL_Rect previewBox = { previewX, previewY + 30, NEXT_PREVIEW_SIZE * BLOCK_SIZE + 10, 
                            NEXT_PREVIEW_SIZE * BLOCK_SIZE + 10 };
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &previewBox);
    
    // Draw next piece inside preview box
    int nextType = tetromino.getNextType();
    const int* nextPiece = tetromino.getNextPiece();
    
    for (int py = 0; py < NEXT_PREVIEW_SIZE; py++) {
        for (int px = 0; px < NEXT_PREVIEW_SIZE; px++) {
            if (nextPiece[py * NEXT_PREVIEW_SIZE + px]) {
                SDL_Rect previewRect = {
                    previewX + 5 + px * BLOCK_SIZE,
                    previewY + 35 + py * BLOCK_SIZE,
                    BLOCK_SIZE - 1,
                    BLOCK_SIZE - 1
                };
                setColorForPieceType(nextType);
                SDL_RenderFillRect(renderer, &previewRect);
            }
        }
    }
}

void Renderer::render(const Board& board, const Tetromino& tetromino, int score) {
    // Clear screen and draw background
    drawBackground();
    
    // Draw game state
    drawBoard(board);
    drawFallingPiece(tetromino, board);
    drawGridLines();
    drawHUD(score, tetromino);
    
    // Present to screen
    SDL_RenderPresent(renderer);
}

void Renderer::renderMenu(int selectedOption) {
    // Simple menu rendering (text rendering disabled)
    SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
    SDL_RenderClear(renderer);
    
    // Draw simple menu boxes
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect titleBox = { 200, 100, 400, 50 };
    SDL_RenderDrawRect(renderer, &titleBox);
    
    SDL_Rect playBox = { 200, 200, 200, 50 };
    SDL_SetRenderDrawColor(renderer, selectedOption == 0 ? 0 : 50, 150, 0, 255);
    SDL_RenderFillRect(renderer, &playBox);
    
    SDL_RenderPresent(renderer);
}

void Renderer::renderGameOver(int finalScore) {
    // Simple game over rendering (text rendering disabled)
    SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
    SDL_RenderClear(renderer);
    
    // Draw simple game over boxes
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_Rect titleBox = { 150, 150, 500, 50 };
    SDL_RenderDrawRect(renderer, &titleBox);
    
    SDL_Rect scoreBox = { 150, 250, 500, 50 };
    SDL_RenderDrawRect(renderer, &scoreBox);
    
    SDL_RenderPresent(renderer);
}
