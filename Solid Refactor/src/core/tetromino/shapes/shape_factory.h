/*
 * ============================================================================
 * FILE: shape_factory.h
 * PURPOSE: Factory for creating Tetromino shapes - Centralizes shape creation
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP) + Factory Pattern
 *   - New shapes can be registered in the factory without modifying
 *     Tetromino, GameEngine, or any other component
 *   - To add a new shape:
 *     1. Create the shape class inheriting from TetriminoShape
 *     2. Register it in ShapeFactory::registerShape()
 *     3. Update NUM_SHAPES constant if adding a new type
 * 
 * CLIENT CODE doesn't care about shape implementation details;
 *   it just requests shapes from the factory.
 * ============================================================================
 */

#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

#include "../../interfaces/tetromino_shape.h"
#include "tetromino_shapes.h"
#include <memory>
#include <cstdlib>
#include <ctime>

class ShapeFactory {
private:
    static constexpr int NUM_SHAPES = 7;  // Update if adding new standard shapes
    static bool initialized;
    
public:
    /**
     * Initialize the factory (sets up random seed)
     * Should be called once at startup
     */
    static void initialize() {
        if (!initialized) {
            srand((unsigned)time(NULL));
            initialized = true;
        }
    }
    
    /**
     * Create a random Tetromino shape
     * 
     * @return Unique pointer to a new TetriminoShape instance
     */
    static std::unique_ptr<TetriminoShape> createRandomShape() {
        int shapeType = rand() % NUM_SHAPES;
        return createShape(shapeType);
    }
    
    /**
     * Create a specific Tetromino shape by type
     * 
     * @param typeId The type ID (0=I, 1=J, 2=L, 3=O, 4=S, 5=T, 6=Z)
     * @return Unique pointer to a new TetriminoShape instance
     * 
     * NOTE: For extensibility, invalid types return I-Piece (safe default)
     */
    static std::unique_ptr<TetriminoShape> createShape(int typeId) {
        switch (typeId % NUM_SHAPES) {
            case 0: return std::make_unique<IPiece>();
            case 1: return std::make_unique<JPiece>();
            case 2: return std::make_unique<LPiece>();
            case 3: return std::make_unique<OPiece>();
            case 4: return std::make_unique<SPiece>();
            case 5: return std::make_unique<TPiece>();
            case 6: return std::make_unique<ZPiece>();
            default: return std::make_unique<IPiece>();  // Safe default
        }
    }
    
    /**
     * Get the total number of shape types available
     * @return Number of available shapes
     */
    static int getNumShapes() {
        return NUM_SHAPES;
    }
};

#endif // SHAPE_FACTORY_H
