/*
 * ============================================================================
 * FILE: game_state.h
 * PURPOSE: GameState enumeration - Defines all possible game states
 *          Extracted from GameEngine as part of Model layer separation
 * 
 * RESPONSIBILITY: Define enumeration for game state machine
 * 
 * DESIGN PRINCIPLE: Single Responsibility Principle (SRP)
 *   - This enum defines ONLY the possible states of the game
 *   - State transitions are handled by Controller/GameEngine
 *   - State data belongs in the Model layer (not GameEngine)
 * 
 * MODEL LAYER COMPONENT:
 *   - GameState is pure data (no logic, no side effects)
 *   - Belongs in Model layer with Board, Tetromino, Scoring
 *   - View/Controller will query and set game state
 *   - No rendering, no input, no game logic
 * ============================================================================
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

/**
 * Enumeration for all possible game states
 * 
 * STATES:
 *   - MENU: Game is displaying main menu, awaiting user selection
 *   - PLAYING: Game is actively running, piece is falling
 *   - GAME_OVER: Game has ended, showing game-over screen
 *   - QUIT: Signal to exit the game loop
 * 
 * INVARIANT:
 *   - Exactly one state is active at any time
 *   - State transitions follow defined rules (Controller responsibility)
 *   - State never changes mid-frame
 */
enum class GameStateEnum {
    MENU,
    PLAYING,
    GAME_OVER,
    QUIT
};

#endif // GAME_STATE_H
