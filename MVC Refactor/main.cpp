/*
 * ============================================================================
 * FILE: main.cpp
 * PURPOSE: Composition Root - MVC Architecture for Tetris Game
 *          CONTROLLER LAYER LOCATION: mvc_refactor/ root
 *
 * RESPONSIBILITY: Create and wire all three MVC layers
 *
 * MVC ARCHITECTURE:
 *   MODEL: Board, Tetromino, ScoringStrategy (in src/core/model/)
 *   VIEW: Renderer, GameStateRenderers (in src/view/)
 *   CONTROLLER: GameEngine, InputHandler (in src/controller/)
 *
 * COMPOSITION ROOT PATTERN:
 *   - All object creation happens in main()
 *   - All wiring/injection happens in main()
 *   - main() is the ONLY place where concrete classes are instantiated
 *   - GameEngine receives only interfaces, not concrete types
 *   - This enforces SOLID principles throughout the application
 *
 * FLOW:
 *   1. Create Model (factories will create Board, Tetromino, ScoringStrategy)
 *   2. Create SDL context (Window, Renderer)
 *   3. Create Input layer (InputHandler implements IInputProvider)
 *   4. Create View layer (GameStateRenderers implement IGameStateRenderer)
 *   5. Create Controller layer (GameEngine coordinates everything)
 *   6. Game loop: update() → render() → display
 *   7. Cleanup (unique_ptr handles automatic destruction)
 *
 * BENEFITS OF THIS ARCHITECTURE:
 *   ✓ Separation of Concerns: Each layer has clear responsibility
 *   ✓ Dependency Inversion: Dependencies flow through interfaces
 *   ✓ Testability: Mock objects can replace any layer
 *   ✓ Extensibility: New renderers/input sources don't need GameEngine changes
 *   ✓ No Circular Dependencies: Model ← Controller ← View
 *   ✓ Clear Data Flow: Input → Controller → Model → View
 *
 * ============================================================================
 */

#include <iostream>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>

#define SDL_MAIN_HANDLED

// Model Layer
#include "src/model/board.h"
#include "src/model/tetromino.h"
#include "src/model/shape_factory.h"
#include "src/model/scoring_factory.h"
#include "src/model/interfaces/scoring_strategy.h"

// View Layer
#include "src/view/renderer.h"
#include "src/view/game_state_renderers.h"
#include "src/view/interfaces/igame_state_renderer.h"

// Controller Layer
#include "src/controller/game_engine.h"
#include "src/controller/input_handler.h"
#include "src/controller/interfaces/iinput_provider.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS)

/**
 * Initialize SDL and create window/renderer
 * 
 * @param window Output: SDL window pointer
 * @param renderer Output: SDL renderer pointer
 * @return true if successful, false on error
 */
bool initializeSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    window = SDL_CreateWindow(
        "Tetris - MVC Architecture",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }
    
    return true;
}

/**
 * Cleanup SDL resources
 */
void cleanupSDL(SDL_Window* window, SDL_Renderer* renderer) {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

/**
 * Main function - Composition Root and Game Loop
 * 
 * ARCHITECTURE:
 *   1. Initialize SDL
 *   2. Create Model layer (via factories)
 *   3. Create View layer (inject SDL renderer)
 *   4. Create Input layer (inject SDL event handler)
 *   5. Create Controller layer (inject View, Input, Model)
 *   6. Game loop: update() → render()
 *   7. Cleanup (unique_ptr auto-destroys)
 */
int main(int argc, char* argv[]) {
    // SDL_main macro for Windows compatibility
    (void)argc;
    (void)argv;
    SDL_Window* window = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    
    // ========== INITIALIZE SDL ==========
    if (!initializeSDL(window, sdlRenderer)) {
        return 1;
    }
    
    // ========== CREATE MODEL LAYER ==========
    // Factories handle creation of shape strategies
    ShapeFactory::initialize();
    std::unique_ptr<ScoringStrategy> scoringStrategy = 
        ScoringFactory::createStrategy(ScoringType::CLASSIC);
    
    // ========== CREATE VIEW LAYER ==========
    // Note: Font rendering disabled - SDL2_ttf library not available
    // TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    
    // Renderer holds SDL context and is owned by this function
    // But is passed to GameStateRenderers for rendering
    auto renderer = std::make_unique<Renderer>(sdlRenderer, nullptr);
    
    // Create game state renderers
    std::vector<std::unique_ptr<IGameStateRenderer>> renderers;
    renderers.push_back(std::make_unique<PlayingRenderer>(renderer.get()));
    renderers.push_back(std::make_unique<GameOverRenderer>(renderer.get()));
    renderers.push_back(std::make_unique<MenuRenderer>(renderer.get()));
    
    // ========== CREATE INPUT LAYER ==========
    auto inputHandler = std::make_unique<InputHandler>();
    IInputProvider* inputProvider = inputHandler.get();  // Non-owning pointer for GameEngine
    
    // ========== CREATE CONTROLLER LAYER ==========
    // GameEngine receives only interfaces, not concrete types
    auto gameEngine = std::make_unique<GameEngine>(
        inputProvider,
        std::move(renderers),
        std::move(scoringStrategy)
    );
    
    // ========== GAME LOOP ==========
    gameEngine->startNewGame();
    bool running = true;
    Uint32 lastFrameTime = SDL_GetTicks();
    
    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 frameDelta = currentTime - lastFrameTime;
        
        // Update game state (read input, mutate model)
        gameEngine->update(currentTime);
        
        // Render game state (view reads const model references)
        gameEngine->render();
        
        // Check quit condition
        if (gameEngine->shouldQuit()) {
            running = false;
        }
        
        // Frame rate limiting
        if (frameDelta < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameDelta);
        }
        
        lastFrameTime = currentTime;
    }
    
    // ========== CLEANUP ==========
    // unique_ptr automatically destroys gameEngine, renderer, inputHandler
    gameEngine = nullptr;
    renderer = nullptr;
    inputHandler = nullptr;
    
    cleanupSDL(window, sdlRenderer);
    
    std::cout << "Game closed cleanly" << std::endl;
    return 0;
}

// Windows entry point wrapper (required for Windows GUI application)
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow) {
    (void)hInstance;
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;
    return main(0, nullptr);
}
#endif
