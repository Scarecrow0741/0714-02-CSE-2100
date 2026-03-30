/*
 * ============================================================================
 * FILE: main.cpp
 * PURPOSE: Main entry point and game loop for Tetris
 * 
 * This file contains:
 *   - SDL2 initialization and window creation
 *   - Font loading (TTF) for text rendering
 *   - Main game loop with state management (MENU, PLAYING, GAME_OVER)
 *   - Event handling for keyboard and mouse input
 *   - Game timing and piece drop logic
 *   - Resource cleanup and SDL shutdown
 * 
 * GAME STATES:
 *   - GAME_STATE_MENU: Main menu with Play/Exit options
 *   - GAME_STATE_PLAYING: Active gameplay with piece movement and controls
 *   - GAME_STATE_GAME_OVER: Game over screen showing final score
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
#include <bits/stdc++.h>
using namespace std;

#define SDL_MAIN_HANDLED

/*
 * FUNCTION: main()
 * PURPOSE: Entry point for the Tetris game
 * 
 * Initialization:
 *   1. Initialize SDL2 library (video and timer subsystems)
 *   2. Initialize SDL2_TTF for font rendering
 *   3. Load Arial font from Windows system fonts
 *   4. Calculate and create the game window
 *   5. Create SDL2 renderer for graphics
 * 
 * Main Loop:
 *   - Processes events (keyboard, mouse, quit)
 *   - Manages three screen states (menu, gameplay, game over)
 *   - Updates game logic each frame
 *   - Renders graphics
 *   - Handles timing for piece drops
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
    
    // Create and initialize game state structure
    GameState game;
    memset(&game, 0, sizeof(GameState));

    // ===== SDL2 INITIALIZATION =====
    // Initialize SDL2 with VIDEO (graphics) and TIMER (timing) subsystems
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
    game.font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    if (!game.font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // ===== WINDOW SETUP =====
    // Calculate window dimensions based on game board size and HUD area
    int board_pixels_w = BOARD_WIDTH * BLOCK_SIZE;     // 20 * 20 = 400 pixels
    int board_pixels_h = BOARD_HEIGHT * BLOCK_SIZE;    // 20 * 20 = 400 pixels
    game.win_w = WINDOW_BORDER + board_pixels_w + HUD_WIDTH;  // Total width
    game.win_h = WINDOW_BORDER + board_pixels_h;                // Total height

    // Create SDL window centered on screen with title "Tetris SDL2"
    game.window = SDL_CreateWindow("Tetris SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  game.win_w, game.win_h, SDL_WINDOW_SHOWN);
    if (!game.window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer for drawing graphics (accelerated + vsync enabled)
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game.renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(game.window);
        SDL_Quit();
        return 1;
    }

    // ===== GAME STATE INITIALIZATION =====
    // Start at the main menu screen
    game.screen_state = GAME_STATE_MENU;
    game.selected_option = MENU_PLAY;
    
    // Timing variables for piece drop logic
    Uint32 last_tick = SDL_GetTicks();           // Previous frame time
    const Uint32 drop_interval_ms = 500;         // Piece drops every 500ms
    Uint32 drop_acc = 0;                          // Accumulated time for drop
    int quit = 0;                                  // Main loop exit flag

    // ===== MAIN GAME LOOP =====
    while (!quit) {
        // ===== MENU STATE =====
        if (game.screen_state == GAME_STATE_MENU) {
            // Draw menu screen with Play/Exit buttons
            draw_menu(game.renderer, game.font, game.selected_option);
            
            SDL_Event ev;
            // Process all pending events
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) {
                    // Window close button clicked
                    quit = 1;
                } else if (ev.type == SDL_MOUSEMOTION) {
                    // Mouse moved - update selected option based on position
                    MenuOption hover = check_menu_click(ev.motion.x, ev.motion.y);
                    if (hover == MENU_PLAY || hover == MENU_EXIT) {
                        game.selected_option = hover;
                    }
                } else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                    // Mouse button clicked
                    if (ev.button.button == SDL_BUTTON_LEFT) {
                        MenuOption clicked = check_menu_click(ev.button.x, ev.button.y);
                        if (clicked == MENU_PLAY) {
                            // Start new game
                            game.screen_state = GAME_STATE_PLAYING;
                            init_game(&game);
                        } else if (clicked == MENU_EXIT) {
                            // Exit application
                            quit = 1;
                        }
                    }
                } else if (ev.type == SDL_KEYDOWN) {
                    // Keyboard input handling
                    switch (ev.key.keysym.sym) {
                        case SDLK_UP:
                        case SDLK_DOWN:
                            // UP/DOWN arrow toggles between menu options
                            game.selected_option = (game.selected_option == MENU_PLAY) ? MENU_EXIT : MENU_PLAY;
                            break;
                        case SDLK_RETURN:
                            // ENTER key activates selected option
                            if (game.selected_option == MENU_PLAY) {
                                game.screen_state = GAME_STATE_PLAYING;
                                init_game(&game);
                            } else {
                                quit = 1;
                            }
                            break;
                    }
                }
            }
            SDL_Delay(16);  // Cap menu at ~60 FPS (16ms per frame)
            continue;       // Skip to next frame without processing gameplay
        }
        
        // ===== GAME OVER STATE =====
        if (game.screen_state == GAME_STATE_GAME_OVER) {
            // Draw game over screen with final score
            draw_game_over(game.renderer, game.font, game.score);
            
            SDL_Event ev;
            // Process all pending events
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) {
                    quit = 1;
                } else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN) {
                    // ENTER key returns to menu
                    game.screen_state = GAME_STATE_MENU;
                }
            }
            SDL_Delay(16);  // Cap at ~60 FPS
            continue;       // Skip to next frame
        }

        // ===== GAMEPLAY STATE =====
        if (game.screen_state == GAME_STATE_PLAYING) {
            SDL_Event ev;
            // Process all pending events (keyboard input during gameplay)
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) {
                    // Window close ends game
                    game.game_over = 1;
                } else if (ev.type == SDL_KEYDOWN) {
                    SDL_Keycode key = ev.key.keysym.sym;
                    switch (key) {
                        // MOVE LEFT
                        case SDLK_a:
                        case SDLK_LEFT:
                            move_piece(&game, -1, 0);  // dx=-1 (left)
                            break;
                        
                        // MOVE RIGHT
                        case SDLK_d:
                        case SDLK_RIGHT:
                            move_piece(&game, 1, 0);   // dx=+1 (right)
                            break;
                        
                        // MOVE DOWN (soft drop)
                        case SDLK_s:
                        case SDLK_DOWN:
                            move_piece(&game, 0, 1);   // dy=+1 (down)
                            break;
                        
                        // ROTATE PIECE
                        case SDLK_w:
                        case SDLK_UP:
                            rotate_piece(&game);
                            break;
                        
                        // QUIT GAME
                        case SDLK_q:
                            game.game_over = 1;
                            break;
                        
                        // HARD DROP (instant drop to bottom)
                        case SDLK_SPACE:
                            // Keep moving down until collision
                            while (!check_collision(&game)) {
                                game.piece_y++;
                            }
                            game.piece_y--;  // Move back one step (before collision)
                            move_piece(&game, 0, 1);  // Finalize the drop
                            break;
                        
                        default:
                            break;
                    }
                }
            }

            // ===== GAME TIMING AND UPDATES =====
            // Get current time in milliseconds
            Uint32 now = SDL_GetTicks();
            Uint32 delta = now - last_tick;  // Time elapsed since last frame
            last_tick = now;
            drop_acc += delta;               // Accumulate time

            // Auto-drop piece every 500ms if enough time has passed
            if (drop_acc >= drop_interval_ms) {
                move_piece(&game, 0, 1);     // Move piece down
                drop_acc = 0;                 // Reset accumulator
            }

            // ===== RENDERING =====
            // Draw current game state
            draw_game_sdl(&game);
            
            // Small delay for frame pacing (~125 FPS cap)
            SDL_Delay(8);
            
            // Check if game ended (piece spawned at top center caused collision)
            if (game.game_over) {
                game.screen_state = GAME_STATE_GAME_OVER;
                game.game_over = 0;  // Reset for next game
            }
        }
    }

    // ===== SHUTDOWN AND CLEANUP =====
    if (game.font) TTF_CloseFont(game.font);
    if (game.renderer) SDL_DestroyRenderer(game.renderer);
    if (game.window) SDL_DestroyWindow(game.window);
    TTF_Quit();   // Shutdown SDL2_TTF
    SDL_Quit();   // Shutdown SDL2
    return 0;
}

