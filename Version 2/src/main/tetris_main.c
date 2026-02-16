/**
 * @file main/tetris_main.c
 * @brief Main entry point and game loop orchestration
 *
 * Initializes SDL2, manages the game state machine, and coordinates
 * the main game loop. Handles all input processing and delegating
 * to appropriate game system (menu, gameplay, game over).
 *
 * @author Tetris Development Team
 * @date February 2026
 * @version 2.0
 */

#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../core/types/tetris_types.h"
#include "../core/types/tetris_constants.h"
#include "../core/board/board_state.h"
#include "../core/board/board_collision.h"
#include "../ui/sdl/sdl_renderer.h"
#include "../ui/menu/game_menu.h"

/**
 * @brief Handles input events for the menu state
 * 
 * Processes mouse and keyboard input during menu screen,
 * updating selected option and transitioning to gameplay
 * or exit when appropriate.
 * 
 * @param game Game state
 * @param quit Pointer to quit flag (set to 1 to exit application)
 * 
 * @return None
 */
static void handleMenuInput(GameState *game, int *quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *quit = 1;
                break;
                
            case SDL_MOUSEMOTION:
            {
                /* Update selection based on hover */
                MenuOption hover = Menu_CheckClickPosition(event.motion.x, event.motion.y);
                if (hover != -1) {
                    game->selected_option = hover;
                }
                break;
            }
            
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    MenuOption clicked = Menu_CheckClickPosition(event.button.x, event.button.y);
                    if (clicked == MENU_PLAY) {
                        game->screen_state = GAME_STATE_PLAYING;
                        Board_InitializeGame(game);
                    } else if (clicked == MENU_EXIT) {
                        *quit = 1;
                    }
                }
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_DOWN:
                        game->selected_option = (game->selected_option == MENU_PLAY) 
                            ? MENU_EXIT : MENU_PLAY;
                        break;
                    case SDLK_RETURN:
                        if (game->selected_option == MENU_PLAY) {
                            game->screen_state = GAME_STATE_PLAYING;
                            Board_InitializeGame(game);
                        } else {
                            *quit = 1;
                        }
                        break;
                    default:
                        break;
                }
                break;
                
            default:
                break;
        }
    }
}

/**
 * @brief Handles input events for the game over state
 * 
 * Waits for player to press ENTER to return to menu,
 * or click window close to exit application.
 * 
 * @param game Game state
 * @param quit Pointer to quit flag (set to 1 to exit application)
 * 
 * @return None
 */
static void handleGameOverInput(GameState *game, int *quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                *quit = 1;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_RETURN) {
                    game->screen_state = GAME_STATE_MENU;
                }
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Handles input events during gameplay
 * 
 * Processes movement, rotation, hard drop, and quit commands.
 * 
 * Controls:
 * - A / LEFT: Move left
 * - D / RIGHT: Move right
 * - S / DOWN: Move down (soft drop)
 * - W / UP: Rotate piece
 * - SPACE: Hard drop (instantly place piece)
 * - Q: Quit to menu
 * 
 * @param game Game state
 * 
 * @return None
 */
static void handleGameplayInput(GameState *game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            game->game_over = 1;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                /* Movement - left */
                case SDLK_a:
                case SDLK_LEFT:
                    Board_MovePiece(game, -1, 0);
                    break;
                    
                /* Movement - right */
                case SDLK_d:
                case SDLK_RIGHT:
                    Board_MovePiece(game, 1, 0);
                    break;
                    
                /* Movement - down (soft drop) */
                case SDLK_s:
                case SDLK_DOWN:
                    Board_MovePiece(game, 0, 1);
                    break;
                    
                /* Rotation */
                case SDLK_w:
                case SDLK_UP:
                    Board_RotatePiece(game);
                    break;
                    
                /* Hard drop - fall until collision */
                case SDLK_SPACE:
                    while (!Board_CheckCollision(game)) {
                        game->piece_y++;
                    }
                    game->piece_y--;
                    Board_MovePiece(game, 0, 1);
                    break;
                    
                /* Quit to menu */
                case SDLK_q:
                    game->game_over = 1;
                    break;
                    
                default:
                    break;
            }
        }
    }
}

/**
 * @brief Main application entry point
 * 
 * Initializes SDL2, creates window and renderer, font, and enters the main
 * game loop. Manages state transitions between menu, gameplay, and game over.
 * 
 * @param argc Command line argument count (unused)
 * @param argv Command line arguments (unused)
 * 
 * @return 0 on success, 1 on error
 */
int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    GameState game;
    memset(&game, 0, sizeof(GameState));

    /* Initialize SDL2 video and timer subsystems */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    /* Initialize SDL2_ttf for font rendering */
    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    /* Load system font */
    game.font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    if (!game.font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    /* Calculate window dimensions based on board size and HUD */
    int board_pixels_w = TETRIS_BOARD_WIDTH * TETRIS_BLOCK_SIZE;
    int board_pixels_h = TETRIS_BOARD_HEIGHT * TETRIS_BLOCK_SIZE;
    game.win_w = TETRIS_WINDOW_BORDER + board_pixels_w + TETRIS_HUD_WIDTH;
    game.win_h = TETRIS_WINDOW_BORDER + board_pixels_h;

    /* Create SDL window */
    game.window = SDL_CreateWindow(
        "Tetris SDL2",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        game.win_w,
        game.win_h,
        SDL_WINDOW_SHOWN
    );
    if (!game.window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* Create SDL renderer */
    game.renderer = SDL_CreateRenderer(
        game.window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!game.renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(game.window);
        SDL_Quit();
        return 1;
    }

    /* Initialize game state to menu screen */
    game.screen_state = GAME_STATE_MENU;
    game.selected_option = MENU_PLAY;
    
    /* Timing for piece drop */
    Uint32 last_tick = SDL_GetTicks();
    Uint32 drop_accumulator = 0;
    int quit = 0;

    /* ========== MAIN GAME LOOP ========== */
    while (!quit) {
        if (game.screen_state == GAME_STATE_MENU) {
            /* Menu state: display menu and process input */
            Menu_DrawMainMenu(game.renderer, game.font, game.selected_option);
            handleMenuInput(&game, &quit);
            SDL_Delay(TETRIS_MENU_FPS_CAP_MS);
        } 
        else if (game.screen_state == GAME_STATE_GAME_OVER) {
            /* Game over state: display final score and wait for menu return */
            Menu_DrawGameOver(game.renderer, game.font, game.score);
            handleGameOverInput(&game, &quit);
            SDL_Delay(TETRIS_MENU_FPS_CAP_MS);
        } 
        else if (game.screen_state == GAME_STATE_PLAYING) {
            /* Active game play state */
            handleGameplayInput(&game);

            /* Update timing for piece drop */
            Uint32 now = SDL_GetTicks();
            Uint32 delta = now - last_tick;
            last_tick = now;
            drop_accumulator += delta;

            /* Auto-drop piece when interval elapses */
            if (drop_accumulator >= TETRIS_DROP_INTERVAL_MS) {
                Board_MovePiece(&game, 0, 1);
                drop_accumulator = 0;
            }

            /* Render current gameplay frame */
            SDLRenderer_DrawGameplay(&game);
            SDL_Delay(TETRIS_GAMEPLAY_FPS_CAP_MS);

            /* Check if game ended and transition to game over screen */
            if (game.game_over) {
                game.screen_state = GAME_STATE_GAME_OVER;
                game.game_over = 0; /* Reset for next game */
            }
        }
    }

    /* ========== CLEANUP ========== */
    if (game.font) TTF_CloseFont(game.font);
    if (game.renderer) SDL_DestroyRenderer(game.renderer);
    if (game.window) SDL_DestroyWindow(game.window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
