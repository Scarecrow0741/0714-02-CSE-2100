/*
 * ============================================================================
 * FILE: tetromino_shapes.h
 * PURPOSE: Concrete implementations of TetriminoShape interface for all 7 Tetrominos
 * 
 * LISKOV SUBSTITUTION PRINCIPLE (LSP) COMPLIANCE:
 *   Every shape in this file satisfies the TetriminoShape contract uniformly.
 *   Clients can substitute any shape for another without changing behavior.
 * 
 *   GUARANTEED PROPERTIES:
 *   [✓] All shapes are 4x4 matrices with exactly 4 cells filled
 *   [✓] All shapes rotate using standard 90° clockwise algorithm
 *   [✓] All shapes have unique type IDs (0-6)
 *   [✓] All shapes have distinct colors
 *   [✓] No shape requires special preconditions for rotation
 *   [✓] No shape throws exceptions (all operations succeed)
 *   [✓] All shapes move uniformly via Tetromino (no shape-specific movement)
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP)
 *   Each shape is self-contained and independent. New shapes can be added
 *   as new subclasses without modifying existing code.
 * 
 * NEW SHAPES: To add a new shape, simply:
 *   1. Create a new class inheriting from TetriminoShape
 *   2. Implement the pure virtual methods
 *   3. Register it in the ShapeFactory
 *   4. Verify LSP compliance: no special movement, rotation, or exceptions
 *   
 *   NO changes needed to Tetromino, GameEngine, Renderer, or Board!
 * ============================================================================
 */

#ifndef TETROMINO_SHAPES_H
#define TETROMINO_SHAPES_H

#include "../../interfaces/tetromino_shape.h"

// ============================================================================
// I-Piece (cyan line)
// LSP Compliance: Standard 4-cell line shape, rotates normally
// ============================================================================
class IPiece : public TetriminoShape {
private:
    static const int SHAPE[PIECE_SIZE][PIECE_SIZE];
    
public:
    const int* getShapeMatrix() const override {
        return (const int*)SHAPE;
    }
    
    int getTypeId() const override { return 0; }
    
    void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const override {
        r = 150; g = 220; b = 255; a = 255;  // Cyan
    }
    
    const char* getName() const override { return "I-Piece"; }
};

// ============================================================================
// J-Piece (blue L-shape)
// LSP Compliance: Standard 4-cell L-shape, rotates normally
// ============================================================================
class JPiece : public TetriminoShape {
private:
    static const int SHAPE[PIECE_SIZE][PIECE_SIZE];
    
public:
    const int* getShapeMatrix() const override {
        return (const int*)SHAPE;
    }
    
    int getTypeId() const override { return 1; }
    
    void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const override {
        r = 0; g = 128; b = 255; a = 255;  // Blue
    }
    
    const char* getName() const override { return "J-Piece"; }
};

// ============================================================================
// L-Piece (orange L-shape)
// LSP Compliance: Standard 4-cell L-shape, rotates normally
// ============================================================================
class LPiece : public TetriminoShape {
private:
    static const int SHAPE[PIECE_SIZE][PIECE_SIZE];
    
public:
    const int* getShapeMatrix() const override {
        return (const int*)SHAPE;
    }
    
    int getTypeId() const override { return 2; }
    
    void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const override {
        r = 255; g = 165; b = 0; a = 255;  // Orange
    }
    
    const char* getName() const override { return "L-Piece"; }
};

// ============================================================================
// O-Piece (yellow square) - THE "SQUARE TEST" FOR LSP
// LSP Compliance: 
//   - Despite visual symmetry, rotate() MUST update internal state (✓ we do)
//   - Postcondition: "rotation state incremented" holds for ALL shapes
//   - Clients should NOT check type to skip rotation for O-pieces
//   - Visually identical rotations do NOT violate LSP (only contract matters)
// ============================================================================
class OPiece : public TetriminoShape {
private:
    static const int SHAPE[PIECE_SIZE][PIECE_SIZE];
    
public:
    const int* getShapeMatrix() const override {
        return (const int*)SHAPE;
    }
    
    int getTypeId() const override { return 3; }
    
    void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const override {
        r = 255; g = 220; b = 0; a = 255;  // Yellow
    }
    
    const char* getName() const override { return "O-Piece"; }
};

// ============================================================================
// S-Piece (green zigzag)
// LSP Compliance: Standard 4-cell S-shape, rotates normally
// ============================================================================
class SPiece : public TetriminoShape {
private:
    static const int SHAPE[PIECE_SIZE][PIECE_SIZE];
    
public:
    const int* getShapeMatrix() const override {
        return (const int*)SHAPE;
    }
    
    int getTypeId() const override { return 4; }
    
    void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const override {
        r = 0; g = 255; b = 0; a = 255;  // Green
    }
    
    const char* getName() const override { return "S-Piece"; }
};

// ============================================================================
// T-Piece (purple T-shape)
// LSP Compliance: Standard 4-cell T-shape, rotates normally
// ============================================================================
class TPiece : public TetriminoShape {
private:
    static const int SHAPE[PIECE_SIZE][PIECE_SIZE];
    
public:
    const int* getShapeMatrix() const override {
        return (const int*)SHAPE;
    }
    
    int getTypeId() const override { return 5; }
    
    void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const override {
        r = 160; g = 32; b = 240; a = 255;  // Purple
    }
    
    const char* getName() const override { return "T-Piece"; }
};

// ============================================================================
// Z-Piece (red zigzag)
// LSP Compliance: Standard 4-cell Z-shape, rotates normally
// ============================================================================
class ZPiece : public TetriminoShape {
private:
    static const int SHAPE[PIECE_SIZE][PIECE_SIZE];
    
public:
    const int* getShapeMatrix() const override {
        return (const int*)SHAPE;
    }
    
    int getTypeId() const override { return 6; }
    
    void getColor(unsigned char& r, unsigned char& g, unsigned char& b, unsigned char& a) const override {
        r = 255; g = 80; b = 80; a = 255;  // Red
    }
    
    const char* getName() const override { return "Z-Piece"; }
};

#endif // TETROMINO_SHAPES_H

