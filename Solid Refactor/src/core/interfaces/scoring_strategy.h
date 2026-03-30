/*
 * ============================================================================
 * FILE: scoring_strategy.h
 * PURPOSE: Abstract base class for scoring strategies - Applies Strategy Pattern
 * 
 * RESPONSIBILITY: Define interface for different scoring systems
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP)
 *   - OPEN for extension: New scoring systems (Level-based, Combo, etc.)
 *     can be added without modifying GameEngine
 *   - CLOSED for modification: GameEngine doesn't change when adding new scoring rules
 * 
 * STRATEGY PATTERN: Different ScoringStrategy implementations allow the game
 *   to switch between Classic, Level-based, Combo-based, or custom scoring
 *   at runtime without code changes to GameEngine or Board.
 * ============================================================================
 */

#ifndef SCORING_STRATEGY_H
#define SCORING_STRATEGY_H

class ScoringStrategy {
public:
    virtual ~ScoringStrategy() = default;
    
    /**
     * Calculate score increase for clearing lines
     * 
     * @param linesCleared Number of lines cleared in this action (1-4)
     * @param currentLevel Current game level (for level-based scoring)
     * @param currentScore Current total score (for combo-based scoring)
     * @return Points to add to the score
     */
    virtual int calculateScore(int linesCleared, int currentLevel = 1, int currentScore = 0) = 0;
    
    /**
     * Get the name of this scoring strategy (for debugging/display)
     * @return Strategy name (e.g., "Classic", "Level-Based", "Combo")
     */
    virtual const char* getName() const = 0;
};

#endif // SCORING_STRATEGY_H
