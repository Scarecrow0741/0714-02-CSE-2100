/*
 * ============================================================================
 * FILE: main.cpp
 * PURPOSE: Composition Root and main entry point for Tetris (DIP Architecture)
 * 
 * COMPOSITION ROOT PATTERN:
 *   This file is the "main" or "root" of the dependency injection container.
 *   It is the ONLY place in the codebase that knows about concrete implementations.
 *   All dependencies are created here and wired together before passing to GameEngine.
 * 
 * RESPONSIBILITY:
 *   - Initialize SDL2 and create window/renderer
 *   - Create concrete implementations:
 *     * InputHandler (concrete IInputProvider implementation)
 *     * Renderer (concrete rendering engine)
 *     * ScoringStrategy (chosen strategy for scoring)
 *     * Game state renderers (PlayingRenderer, MenuRenderer, GameOverRenderer)
 *   - Wire all dependencies into GameEngine via constructor injection
 *   - Run the main game loop
 *   - Handle cleanup
 * 
 * DEPENDENCY INVERSION PRINCIPLE (DIP):
 *   - GameEngine has NO knowledge of InputHandler, specific ScoringStrategy, etc.
 *   - Only this file knows how to create the concrete implementations
 *   - All wiring happens in one place (easy to modify, add new implementations)
 *   - Testing: Can create different main() for testing with mock implementations
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
#include "../input/input_handler.h"
#include "../ui/renderer/renderer.h"
#include "../ui/renderers/game_state_renderers.h"
#include "../core/scoring/scoring_factory.h"
#include <bits/stdc++.h>
#include <memory>
using namespace std;

#define SDL_MAIN_HANDLED

/*
 * FUNCTION: createGameDependencies()
 * PURPOSE: Dependency Injection Factory - Creates all GameEngine dependencies
 * 
 * This is where all concrete implementations are instantiated.
 * This is the ONLY place that knows about concrete classes like InputHandler,
 * specific ScoringStrategy implementations, and game renderers.
 * 
 * RETURN: std::array containing [inputProvider, renderer, scoringStrategy, 
 *                                 playingRenderer, menuRenderer, gameOverRenderer]
 *         (We return them via out parameters instead of tuple for C++11 compatibility)
 */
void createGameDependencies(
    SDL_Renderer* sdlRenderer,
    TTF_Font* sdlFont,
    std::unique_ptr<IInputProvider>& outInputProvider,
    std::unique_ptr<Renderer>& outRenderer,
    std::unique_ptr<ScoringStrategy>& outScoringStrategy,
    std::unique_ptr<GameRenderer>& outPlayingRenderer,
    std::unique_ptr<GameRenderer>& outMenuRenderer,
    std::unique_ptr<GameRenderer>& outGameOverRenderer
) {
    // ===== CREATE CONCRETE IMPLEMENTATIONS =====
    
    // 1. Create InputHandler (concrete implementation of IInputProvider)
    outInputProvider = std::make_unique<InputHandler>();
    
    // 2. Create Renderer (concrete rendering engine)
    outRenderer = std::make_unique<Renderer>(sdlRenderer, sdlFont);
    
    // 3. Create ScoringStrategy (using factory pattern to choose strategy)
    //    Currently configured for CLASSIC scoring, but can easily be LEVEL_BASED, etc.
    outScoringStrategy = ScoringFactory::createStrategy(ScoringType::CLASSIC);
    
    // 4. Create game state renderers (concrete implementations)
    //    Each renderer knows how to render a specific game state
    Renderer* rawRenderer = outRenderer.get();  // Get raw pointer for renderers to use
    outPlayingRenderer = std::make_unique<PlayingRenderer>(rawRenderer);
    outMenuRenderer = std::make_unique<MenuRenderer>(rawRenderer);
    outGameOverRenderer = std::make_unique<GameOverRenderer>(rawRenderer);
}
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
    SDL_Window* window = SDL_CreateWindow("Tetris SDL2 - DIP Refactor",
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
    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, 
                                                   SDL_RENDERER_ACCELERATED | 
                                                   SDL_RENDERER_PRESENTVSYNC);
    if (!sdlRenderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_CloseFont(font);
        SDL_Quit();
        return 1;
    }

    // ===== DEPENDENCY INJECTION - Create all GameEngine dependencies =====
    // This is the COMPOSITION ROOT - the only place that knows about concrete implementations
    std::unique_ptr<IInputProvider> inputProvider;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<ScoringStrategy> scoringStrategy;
    std::unique_ptr<GameRenderer> playingRenderer;
    std::unique_ptr<GameRenderer> menuRenderer;
    std::unique_ptr<GameRenderer> gameOverRenderer;
    
    createGameDependencies(
        sdlRenderer,
        font,
        inputProvider,
        renderer,
        scoringStrategy,
        playingRenderer,
        menuRenderer,
        gameOverRenderer
    );
    
    // ===== GAME INITIALIZATION - Wire dependencies into GameEngine =====
    // GameEngine receives all its dependencies. It knows nothing about concrete implementations.
    // This follows the Dependency Inversion Principle: High-level modules depend on abstractions.
    GameEngine gameEngine(
        std::move(inputProvider),
        std::move(renderer),
        std::move(scoringStrategy),
        std::move(playingRenderer),
        std::move(menuRenderer),
        std::move(gameOverRenderer)
    );
    
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
            draw_menu(sdlRenderer, font, selectedOption);

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
            draw_game_over(sdlRenderer, font, gameEngine.getScore());

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
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}

