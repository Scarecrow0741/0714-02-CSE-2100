/*
 * ============================================================================
 * FILE: main.cpp
 * PURPOSE: Main entry point and game loop for Tetris (OOP Architecture)
 * 
 * ARCHITECTURE:
 *   - Uses GameEngine class to orchestrate game components
 *   - GameEngine coordinates: Board, Tetromino, InputHandler, Renderer
 *   - Main loop focuses on SDL initialization and state management
 * 
 * This file contains:
 *   - SDL2 initialization and window creation
 *   - Font loading (TTF) for text rendering
 *   - Main game loop with state management (MENU, PLAYING, GAME_OVER)
 *   - Menu and game over screen handling (legacy functions)
 *   - Resource cleanup and SDL shutdown
 * 
 * GAME STATES:
 *   - MENU: Main menu with Play/Exit options
 *   - PLAYING: Active gameplay (delegated to GameEngine)
 *   - GAME_OVER: Game over screen showing final score
 *   - QUIT: Exit application
 * 
 * CONTROL SCHEME:
 *   - A/LEFT ARROW: Move left
 *   - D/RIGHT ARROW: Move right
 *   - S/DOWN ARROW: Move down
 *   - W/UP ARROW: Rotate piece
 *   - SPACE: Hard drop (instant drop to bottom)
 *   - Q: Quit game
 *   - MOUSE: Click menu options
 * ============================================================================
 */

#include "../include/tetris.h"
#include "../core/game_engine/game_engine.h"
#include <bits/stdc++.h>
using namespace std;

#define SDL_MAIN_HANDLED

/*
 * FUNCTION: main()
 * PURPOSE: Entry point for the Tetris game with OOP architecture
 * 
 * Initialization:
 *   1. Initialize SDL2 library (video and timer subsystems)
 *   2. Initialize SDL2_TTF for font rendering
 *   3. Load Arial font from Windows system fonts
 *   4. Create the game window
 *   5. Create SDL2 renderer for graphics
 *   6. Instantiate GameEngine to manage gameplay
 * 
 * Main Loop:
 *   - Menu State: Display menu, handle menu selection
 *   - Playing State: GameEngine handles all game logic and rendering
 *   - Game Over State: Display final score, allow return to menu
 * 
 * Cleanup:
 *   - Closes font and destroys renderer/window
 *   - Shuts down SDL2 and TTF subsystems
 * 
 * RETURN: 0 on success, 1 on error
 */
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    // ===== SDL2 INITIALIZATION =====
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL2_TTF for font rendering
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // Load Arial font from Windows system fonts directory (size 24)
    TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    if (!font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // ===== WINDOW SETUP =====
    int boardPixelsW = BOARD_WIDTH * BLOCK_SIZE;      // 20 * 20 = 400 pixels
    int boardPixelsH = BOARD_HEIGHT * BLOCK_SIZE;     // 20 * 20 = 400 pixels
    int winW = WINDOW_BORDER + boardPixelsW + HUD_WIDTH;  // Total width
    int winH = WINDOW_BORDER + boardPixelsH;               // Total height

    // Create SDL window centered on screen with title "Tetris SDL2"
    SDL_Window* window = SDL_CreateWindow("Tetris SDL2 - OOP Refactor",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         winW, winH,
                                         SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        SDL_Quit();
        return 1;
    }

    // Create renderer for drawing graphics (accelerated + vsync enabled)
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
                                               SDL_RENDERER_ACCELERATED | 
                                               SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        SDL_Quit();
        return 1;
    }

    // ===== GAME INITIALIZATION =====
    // Create GameEngine instance to manage gameplay
    GameEngine gameEngine(renderer, font);
    
    // Start at the main menu
    GameScreenState screenState = GAME_STATE_MENU;
    MenuOption selectedOption = MENU_PLAY;
    int quitApp = 0;

    // Timing variables
    Uint32 lastTick = SDL_GetTicks();

    // ===== MAIN GAME LOOP =====
    while (!quitApp && !gameEngine.shouldQuit()) {
        Uint32 currentTime = SDL_GetTicks();

        // ===== MENU STATE =====
        if (screenState == GAME_STATE_MENU) {
            draw_menu(renderer, font, selectedOption);

            SDL_Event ev;
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) {
                    quitApp = 1;
                } else if (ev.type == SDL_MOUSEMOTION) {
                    MenuOption hover = check_menu_click(ev.motion.x, ev.motion.y);
                    if (hover == MENU_PLAY || hover == MENU_EXIT) {
                        selectedOption = hover;
                    }
                } else if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
                    MenuOption clicked = check_menu_click(ev.button.x, ev.button.y);
                    if (clicked == MENU_PLAY) {
                        screenState = GAME_STATE_PLAYING;
                        gameEngine.startNewGame();
                    } else if (clicked == MENU_EXIT) {
                        quitApp = 1;
                    }
                } else if (ev.type == SDL_KEYDOWN) {
                    switch (ev.key.keysym.sym) {
                        case SDLK_UP:
                        case SDLK_DOWN:
                            selectedOption = (selectedOption == MENU_PLAY) ? MENU_EXIT : MENU_PLAY;
                            break;
                        case SDLK_RETURN:
                            if (selectedOption == MENU_PLAY) {
                                screenState = GAME_STATE_PLAYING;
                                gameEngine.startNewGame();
                            } else {
                                quitApp = 1;
                            }
                            break;
                    }
                }
            }
            SDL_Delay(16);  // Cap at ~60 FPS
            continue;
        }

        // ===== GAMEPLAY STATE =====
        if (screenState == GAME_STATE_PLAYING) {
            // Update and render via GameEngine
            gameEngine.update(currentTime);
            gameEngine.render();
            
            // Check if game ended
            if (gameEngine.getGameState() == GameStateEnum::GAME_OVER) {
                screenState = GAME_STATE_GAME_OVER;
            }

            SDL_Delay(8);  // Frame pacing
            continue;
        }

        // ===== GAME OVER STATE =====
        if (screenState == GAME_STATE_GAME_OVER) {
            draw_game_over(renderer, font, gameEngine.getScore());

            SDL_Event ev;
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) {
                    quitApp = 1;
                } else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN) {
                    screenState = GAME_STATE_MENU;
                }
            }
            SDL_Delay(16);  // Cap at ~60 FPS
            continue;
        }
    }

    // ===== SHUTDOWN AND CLEANUP =====
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

