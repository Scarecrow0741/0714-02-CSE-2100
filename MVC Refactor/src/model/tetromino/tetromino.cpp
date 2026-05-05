/*
 * ============================================================================
 * FILE: tetromino.cpp
 * PURPOSE: Implementation of Tetromino class (OCP-COMPLIANT)
 * 
 * DESIGN NOTE: The Tetromino class now uses composition with TetriminoShape
 * objects instead of hardcoded shape arrays. This allows new shapes to be
 * added without modifying this file.
 * ============================================================================
 */

#include "tetromino.h"
#include "../tetromino/shape_factory.h"
#include <cstring>

Tetromino::Tetromino()
    : currentRotation(0), nextRotation(0), posX(0), posY(0), lastRotationState(-1) {
    // Initialize with random first shape
    ShapeFactory::initialize();
    currentShape = ShapeFactory::createRandomShape();
    nextShape = ShapeFactory::createRandomShape();
    
    // Initialize rotated piece cache
    memset(rotatedPiece, 0, sizeof(rotatedPiece));
}

Tetromino::~Tetromino() {
    // Unique pointers clean up automatically
}

void Tetromino::initialize() {
    // Generate the first next piece type
    nextShape = ShapeFactory::createRandomShape();
    nextRotation = 0;
    
    // Spawn the first active piece
    spawnNewPiece();
}

void Tetromino::spawnNewPiece() {
    // Move next piece to become the current piece
    currentShape = std::move(nextShape);
    currentRotation = nextRotation;
    
    // Generate a new random piece for the next_piece preview
    nextShape = ShapeFactory::createRandomShape();
    nextRotation = 0;
    
    // Reset position to top-center (will be adjusted by GameEngine)
    posX = 8;  // (BOARD_WIDTH/2) - 2 = 10 - 2 = 8
    posY = 0;
    
    // Invalidate rotation cache
    lastRotationState = -1;
}

void Tetromino::setPosition(int x, int y) {
    posX = x;
    posY = y;
}

void Tetromino::moveBy(int dx, int dy) {
    posX += dx;
    posY += dy;
}

void Tetromino::rotateCW(int piece[PIECE_SIZE][PIECE_SIZE]) const {
    // Rotate 90 degrees clockwise
    // Formula: rotated[x][3-y] = original[y][x]
    int rotated[PIECE_SIZE][PIECE_SIZE];
    for (int y = 0; y < PIECE_SIZE; y++) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            rotated[x][3 - y] = piece[y][x];
        }
    }
    // Copy back
    memcpy(piece, rotated, sizeof(rotated));
}

void Tetromino::copyPiece(const int source[PIECE_SIZE][PIECE_SIZE],
                          int dest[PIECE_SIZE][PIECE_SIZE]) const {
    for (int y = 0; y < PIECE_SIZE; y++) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            dest[y][x] = source[y][x];
        }
    }
}

void Tetromino::rotate() {
    // LSP GUARANTEE: This method updates rotation state uniformly for ALL shapes
    // IMPORTANT: Even O-piece (square) must update its rotation counter!
    // This fulfills the postcondition: "rotation state is always incremented"
    // Clients should NEVER check shape type to skip or handle rotation specially
    
    currentRotation = (currentRotation + 1) % 4;
    
    // Invalidate cache so next getPieceAt/getCurrentPiece call updates
    lastRotationState = -1;
    
    // Note: Collision detection and wall-kick is handled by GameEngine
    // (delegated by contract, not handled by shape-specific logic)
}

void Tetromino::updateRotatedPiece() const {
    if (lastRotationState == currentRotation) {
        return;  // Cache is valid
    }
    
    // Get base shape
    const int* baseShape = currentShape->getShapeMatrix();
    
    // Copy to working array
    int working[PIECE_SIZE][PIECE_SIZE];
    for (int i = 0; i < PIECE_SIZE; i++) {
        for (int j = 0; j < PIECE_SIZE; j++) {
            working[i][j] = baseShape[i * PIECE_SIZE + j];
        }
    }
    
    // Apply rotations
    for (int rot = 0; rot < currentRotation; rot++) {
        rotateCW(working);
    }
    
    // Cache the result
    copyPiece(working, (int(*)[PIECE_SIZE])rotatedPiece);
    lastRotationState = currentRotation;
}

int Tetromino::getPieceAt(int x, int y) const {
    if (x < 0 || x >= PIECE_SIZE || y < 0 || y >= PIECE_SIZE) {
        return 0;
    }
    
    updateRotatedPiece();
    return rotatedPiece[y][x];
}

const int* Tetromino::getCurrentPiece() const {
    updateRotatedPiece();
    return (const int*)rotatedPiece;
}

const int* Tetromino::getNextPiece() const {
    // Next piece is always in base rotation (no rotation)
    return nextShape->getShapeMatrix();
}

int Tetromino::getCurrentType() const {
    return currentShape ? currentShape->getTypeId() : 0;
}

int Tetromino::getNextType() const {
    return nextShape ? nextShape->getTypeId() : 0;
}

const int* Tetromino::getPieceShape(int type) {
    // Static compatibility method using factory
    auto shape = ShapeFactory::createShape(type);
    return shape->getShapeMatrix();
}

// Static variable initialization for ShapeFactory
bool ShapeFactory::initialized = false;
