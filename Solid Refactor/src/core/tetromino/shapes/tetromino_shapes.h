/*
 * ============================================================================
 * FILE: tetromino_shapes.h
 * PURPOSE: Concrete implementations of TetriminoShape interface for all 7 Tetrominos
 * 
 * DESIGN PRINCIPLE: Open/Closed Principle (OCP)
 *   Each shape is self-contained and independent. New shapes can be added
 *   as new subclasses without modifying existing code.
 * 
 * NEW SHAPES: To add a new shape, simply:
 *   1. Create a new class inheriting from TetriminoShape
 *   2. Implement the pure virtual methods
 *   3. Register it in the ShapeFactory
 *   
 *   NO changes needed to Tetromino, GameEngine, Renderer, or Board!
 * ============================================================================
 */

#ifndef TETROMINO_SHAPES_H
#define TETROMINO_SHAPES_H

#include "../../interfaces/tetromino_shape.h"

// ============================================================================
// I-Piece (cyan line)
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
// O-Piece (yellow square)
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
