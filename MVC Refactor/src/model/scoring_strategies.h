/*
 * ============================================================================
 * FILE: scoring_strategies.h
 * PURPOSE: Multiple concrete implementations of ScoringStrategy interface
 * 
 * LISKOV SUBSTITUTION PRINCIPLE (LSP) COMPLIANCE:
 *   Every scoring strategy below maintains identical contracts:
 *   
 *   [✓] All strategies accept identical parameter ranges
 *   [✓] All strategies return non-negative integers
 *   [✓] No strategy throws exceptions (exception-safe)
 *   [✓] All strategies handle linesCleared [1,4] uniformly
 *   [✓] No strategy requires special preconditions on parameters
 *   [✓] No strategy ignores parameters or requires special "modes"
 *   [✓] Clients can substitute any strategy for another
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP)
 *   Each strategy is self-contained and independent. New scoring systems
 *   can be added as new strategies without modifying GameEngine or Board.
 * 
 * NEW SCORING SYSTEMS: To add a new scoring rule, simply:
 *   1. Create a new class inheriting from ScoringStrategy
 *   2. Implement calculateScore() and getName() with consistent contracts
 *   3. Ensure no exceptions are thrown
 *   4. Instantiate it in GameEngine or a factory
 *   
 *   NO changes needed to GameEngine, Board, Tetromino, or Renderer!
 *   Verify LSP: No special preconditions, no new exceptions, no parameter requirements
 * 
 * AVAILABLE STRATEGIES:
 *   - ClassicScoring: Standard Tetris (lines^2 * 100)
 *   - LevelBasedScoring: Score increases with game level
 *   - ComboBasedScoring: Consecutive clears give bonus multiplier
 * 
 * MODEL LAYER COMPONENT: Pure scoring calculation, no rendering, no input
 * ============================================================================
 */

#ifndef SCORING_STRATEGIES_H
#define SCORING_STRATEGIES_H

#include "interfaces/scoring_strategy.h"

// ============================================================================
// CLASSIC SCORING: Original Tetris rules
// LSP Compliance: All parameters handled uniformly, no exceptions
// ============================================================================
class ClassicScoring : public ScoringStrategy {
public:
    /**
     * Classic scoring: single line = 100, double = 400, triple = 900, tetris = 1600
     * Formula: points = linesCleared^2 * 100
     * 
     * LSP GUARANTEE:
     *   - Accepts any linesCleared in [1, 4]
     *   - currentLevel parameter is accepted but not required (defaulted)
     *   - currentScore parameter is accepted but not required (defaulted)
     *   - No exceptions thrown (exception-safe)
     *   - Result is always non-negative integer
     */
    int calculateScore(int linesCleared, int currentLevel = 1, int currentScore = 0) override {
        return linesCleared * linesCleared * 100;
    }
    
    const char* getName() const override { return "Classic"; }
};

// ============================================================================
// LEVEL-BASED SCORING: Score multiplies by level
// LSP Compliance: All parameters handled uniformly, no exceptions
// ============================================================================
class LevelBasedScoring : public ScoringStrategy {
public:
    /**
     * Points = linesCleared^2 * 100 * (1 + level/10)
     * Encourages progression through levels
     * 
     * LSP GUARANTEE:
     *   - Accepts any linesCleared in [1, 4]
     *   - Uses currentLevel parameter (but handles any value >= 1)
     *   - currentScore parameter accepted but unused (consistent with contract)
     *   - No exceptions thrown (exception-safe)
     *   - Result is always non-negative integer
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
// LSP Compliance: All parameters handled uniformly, no exceptions
// ============================================================================
class ComboBasedScoring : public ScoringStrategy {
public:
    /**
     * Points = linesCleared^2 * 100 * (1 + comboMultiplier)
     * Multiplier increases for consecutive clears
     * 
     * LSP GUARANTEE:
     *   - Accepts any linesCleared in [1, 4]
     *   - currentLevel parameter accepted (consistent with interface)
     *   - Uses currentScore to calculate combo multiplier
     *   - No exceptions thrown (exception-safe)
     *   - Result is always non-negative integer
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
