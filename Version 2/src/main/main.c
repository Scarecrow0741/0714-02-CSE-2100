#include "../include/tetris.h"
#include <stdio.h>
#include <string.h>

#define SDL_MAIN_HANDLED

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    GameState game;
    memset(&game, 0, sizeof(GameState));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    game.font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    if (!game.font) {
        fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int board_pixels_w = BOARD_WIDTH * BLOCK_SIZE;
    int board_pixels_h = BOARD_HEIGHT * BLOCK_SIZE;
    game.win_w = WINDOW_BORDER + board_pixels_w + HUD_WIDTH;
    game.win_h = WINDOW_BORDER + board_pixels_h;

    game.window = SDL_CreateWindow("Tetris SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  game.win_w, game.win_h, SDL_WINDOW_SHOWN);
    if (!game.window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!game.renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(game.window);
        SDL_Quit();
        return 1;
    }

    // Initialize game state
    game.screen_state = GAME_STATE_MENU;
    game.selected_option = MENU_PLAY;
    
    Uint32 last_tick = SDL_GetTicks();
    const Uint32 drop_interval_ms = 500; 
    Uint32 drop_acc = 0;
    int quit = 0;

    while (!quit) {
        if (game.screen_state == GAME_STATE_MENU) {
            draw_menu(game.renderer, game.font, game.selected_option);
            SDL_Event ev;
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) {
                    quit = 1;
                } else if (ev.type == SDL_MOUSEMOTION) {
                    // Highlight option under mouse
                    MenuOption hover = check_menu_click(ev.motion.x, ev.motion.y);
                    if (hover == MENU_PLAY || hover == MENU_EXIT) {
                        game.selected_option = hover;
                    }
                } else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                    if (ev.button.button == SDL_BUTTON_LEFT) {
                        MenuOption clicked = check_menu_click(ev.button.x, ev.button.y);
                        if (clicked == MENU_PLAY) {
                            game.screen_state = GAME_STATE_PLAYING;
                            init_game(&game);
                        } else if (clicked == MENU_EXIT) {
                            quit = 1;
                        }
                    }
                } else if (ev.type == SDL_KEYDOWN) {
                    switch (ev.key.keysym.sym) {
                        case SDLK_UP:
                        case SDLK_DOWN:
                            game.selected_option = (game.selected_option == MENU_PLAY) ? MENU_EXIT : MENU_PLAY;
                            break;
                        case SDLK_RETURN:
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
            SDL_Delay(16); // Cap menu fps
            continue;
        }
        
        if (game.screen_state == GAME_STATE_GAME_OVER) {
            draw_game_over(game.renderer, game.font, game.score);
            SDL_Event ev;
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) {
                    quit = 1;
                } else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_RETURN) {
                    game.screen_state = GAME_STATE_MENU;
                }
            }
            SDL_Delay(16); // Cap game over screen fps
            continue;
        }

        // Normal gameplay
        if (game.screen_state == GAME_STATE_PLAYING) {
        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                game.game_over = 1;
            } else if (ev.type == SDL_KEYDOWN) {
                SDL_Keycode key = ev.key.keysym.sym;
                switch (key) {
                    case SDLK_a:
                    case SDLK_LEFT: move_piece(&game, -1, 0); break;
                    case SDLK_d:
                    case SDLK_RIGHT: move_piece(&game, 1, 0); break;
                    case SDLK_s:
                    case SDLK_DOWN: move_piece(&game, 0, 1); break;
                    case SDLK_w:
                    case SDLK_UP: rotate_piece(&game); break;
                    case SDLK_q: game.game_over = 1; break;
                    case SDLK_SPACE:
                        while (!check_collision(&game)) {
                            game.piece_y++;
                        }
                        game.piece_y--;
                        move_piece(&game, 0, 1);
                        break;
                    default: break;
                }
            }
        }

        Uint32 now = SDL_GetTicks();
        Uint32 delta = now - last_tick;
        last_tick = now;
        drop_acc += delta;

        if (drop_acc >= drop_interval_ms) {
            move_piece(&game, 0, 1);
            drop_acc = 0;
        }

        draw_game_sdl(&game);
            SDL_Delay(8);
            
            if (game.game_over) {
                game.screen_state = GAME_STATE_GAME_OVER;
                game.game_over = 0; // Reset for next game
            }
        }
    }

    if (game.font) TTF_CloseFont(game.font);
    if (game.renderer) SDL_DestroyRenderer(game.renderer);
    if (game.window) SDL_DestroyWindow(game.window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
