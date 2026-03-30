/*
 * ============================================================================
 * FILE: scoring_strategies.h
 * PURPOSE: Multiple concrete implementations of ScoringStrategy interface
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP)
 *   Each strategy is self-contained and independent. New scoring systems
 *   can be added as new strategies without modifying GameEngine or Board.
 * 
 * NEW SCORING SYSTEMS: To add a new scoring rule, simply:
 *   1. Create a new class inheriting from ScoringStrategy
 *   2. Implement calculateScore() with your custom logic
 *   3. Instantiate it in GameEngine
 *   
 *   NO changes needed to GameEngine, Board, Tetromino, or Renderer!
 * 
 * AVAILABLE STRATEGIES:
 *   - ClassicScoring: Standard Tetris (lines^2 * 100)
 *   - LevelBasedScoring: Score increases with game level
 *   - ComboBasedScoring: Consecutive clears give bonus multiplier
 * ============================================================================
 */

#ifndef SCORING_STRATEGIES_H
#define SCORING_STRATEGIES_H

#include "../interfaces/scoring_strategy.h"

// ============================================================================
// CLASSIC SCORING: Original Tetris rules
// ============================================================================
class ClassicScoring : public ScoringStrategy {
public:
    /**
     * Classic scoring: single line = 100, double = 400, triple = 900, tetris = 1600
     * Formula: points = linesCleared^2 * 100
     */
    int calculateScore(int linesCleared, int currentLevel = 1, int currentScore = 0) override {
        return linesCleared * linesCleared * 100;
    }
    
    const char* getName() const override { return "Classic"; }
};

// ============================================================================
// LEVEL-BASED SCORING: Score multiplies by level
// ============================================================================
class LevelBasedScoring : public ScoringStrategy {
public:
    /**
     * Points = linesCleared^2 * 100 * (1 + level/10)
     * Encourages progression through levels
     */
    int calculateScore(int linesCleared, int currentLevel = 1, int currentScore = 0) override {
        int baseScore = linesCleared * linesCleared * 100;
        float multiplier = 1.0f + (currentLevel / 10.0f);
        return (int)(baseScore * multiplier);
    }
    
    const char* getName() const override { return "Level-Based"; }
};

// ============================================================================
// COMBO-BASED SCORING: Consecutive clears give increasing bonuses
// ============================================================================
class ComboBasedScoring : public ScoringStrategy {
public:
    /**
     * Points = linesCleared^2 * 100 * (1 + comboMultiplier)
     * Multiplier increases for consecutive clears
     * 
     * NOTE: This simple version doesn't track combo state itself.
     * For full combo tracking, GameEngine would need to track combo count
     * and pass it as currentScore or additional parameter.
     */
    int calculateScore(int linesCleared, int currentLevel = 1, int currentScore = 0) override {
        int baseScore = linesCleared * linesCleared * 100;
        // Simple multiplier: every 400 points adds 10% bonus (simulates combo)
        float comboMultiplier = 1.0f + (currentScore / 4000.0f) * 0.1f;
        return (int)(baseScore * comboMultiplier);
    }
    
    const char* getName() const override { return "Combo-Based"; }
};

#endif // SCORING_STRATEGIES_H
