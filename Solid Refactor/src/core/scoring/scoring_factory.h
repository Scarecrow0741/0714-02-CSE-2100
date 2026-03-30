/*
 * ============================================================================
 * FILE: scoring_factory.h
 * PURPOSE: Factory for creating ScoringStrategy instances
 * 
 * DESIGN PRINCIPLE: Factory Pattern + OCP
 *   Centralizes strategy creation. To add new scoring systems:
 *   1. Create the strategy class
 *   2. Add a factory method
 *   3. Update the enum if needed
 * 
 * LSP COMPLIANCE:
 *   All strategies created by this factory maintain identical contracts.
 *   Clients can substitute strategies at runtime without code changes.
 *   
 *   GUARANTEED: All returned strategies are substitutable:
 *   - Same parameter contract (linesCleared, level, score)
 *   - Same return type (non-negative int)
 *   - No exceptions
 *   - No special preconditions or "modes"
 * ============================================================================
 */

#ifndef SCORING_FACTORY_H
#define SCORING_FACTORY_H

#include "../interfaces/scoring_strategy.h"
#include "scoring_strategies.h"
#include <memory>

enum class ScoringType {
    CLASSIC,
    LEVEL_BASED,
    COMBO_BASED
};

class ScoringFactory {
public:
    /**
     * Create a scoring strategy by type
     * 
     * LSP GUARANTEE: Any strategy returned maintains identical contract
     * @param type The scoring strategy type to create
     * @return Unique pointer to a new ScoringStrategy instance (substitutable)
     */
    static std::unique_ptr<ScoringStrategy> createStrategy(ScoringType type) {
        switch (type) {
            case ScoringType::CLASSIC:
                return std::make_unique<ClassicScoring>();
            case ScoringType::LEVEL_BASED:
                return std::make_unique<LevelBasedScoring>();
            case ScoringType::COMBO_BASED:
                return std::make_unique<ComboBasedScoring>();
            default:
                return std::make_unique<ClassicScoring>();  // Safe default
        }
    }
};

#endif // SCORING_FACTORY_H
