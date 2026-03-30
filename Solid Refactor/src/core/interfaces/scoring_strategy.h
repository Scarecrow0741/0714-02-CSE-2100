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
 * LISKOV SUBSTITUTION PRINCIPLE (LSP):
 *   - All subclasses MUST maintain the contracts below
 *   - Clients should be able to substitute any ScoringStrategy for another
 *     without changing program correctness
 *   - All strategies accept valid game parameters
 *   - All strategies return non-negative score values
 *   - No strategy throws exceptions not declared in interface
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
     * PRECONDITIONS:
     *   - linesCleared is in range [1, 4] (valid Tetris line clears)
     *   - currentLevel is in range [1, 999] (reasonable game levels)
     *   - currentScore is in range [0, INT_MAX] (non-negative accumulated score)
     * 
     * POSTCONDITIONS:
     *   - Returns non-negative integer (score cannot be negative)
     *   - Return value is deterministic for same inputs
     *   - Return value increases monotonically with linesCleared
     *   - Return value is reasonable (not exploitable, not too large)
     *   - Current parameters do NOT have preconditions beyond ranges above
     *   - No parameter requires special "power-up" or "mode" to process
     *   - No strategy-specific constraints on linesCleared values
     * 
     * LSP GUARANTEE:
     *   - Any strategy can handle any line count [1,4] without failure
     *   - No strategy throws exceptions for valid parameter ranges
     *   - No strategy requires checking type before calling
     *   - No strategy requires initialization beyond construction
     *   - Parameter names (level, score) are hints; actual behavior depends
     *     on the implementation but parameters are never mandatory
     * 
     * @param linesCleared Number of lines cleared in this action (1-4, or 0 for no clear)
     * @param currentLevel Current game level (for level-based scoring, default=1)
     * @param currentScore Current total score (for combo-based scoring, default=0)
     * @return Points to add to the score (>= 0)
     */
    virtual int calculateScore(int linesCleared, int currentLevel = 1, int currentScore = 0) = 0;
    
    /**
     * Get the human-readable name of this scoring strategy (for debugging/display)
     * 
     * PRECONDITIONS:
     *   - Object is properly constructed
     * 
     * POSTCONDITIONS:
     *   - Returns valid C-string (never nullptr)
     *   - String is null-terminated
     *   - String describes the strategy (e.g., "Classic", "Level-Based")
     *   - Name is consistent across multiple calls
     * 
     * LSP GUARANTEE:
     *   - Name is for debugging only; never used for behavior decisions
     * 
     * @return Strategy name (e.g., "Classic", "Level-Based", "Combo")
     */
    virtual const char* getName() const = 0;
};

#endif // SCORING_STRATEGY_H
