/*
 * ============================================================================
 * FILE: tetromino.h
 * PURPOSE: Tetromino class - Manages piece shapes, rotation, and position
 * 
 * RESPONSIBILITY: Handle all piece-related data and transformations
 * 
 * SINGLE RESPONSIBILITY: This class is responsible ONLY for:
 *   - Storing piece shape definitions (all 7 Tetromino types)
 *   - Managing current piece rotation state
 *   - Managing piece position (x, y coordinates)
 *   - Rotating pieces
 *   - Providing piece data for collision detection and rendering
 * 
 * This class DOES NOT:
 *   - Perform collision detection
 *   - Know about the Board
 *   - Know about SDL/rendering
 *   - Manage game rules or scoring
 * ============================================================================
 */

#ifndef TETROMINO_H
#define TETROMINO_H

#define PIECE_SIZE 4

class Tetromino {
private:
    // All 7 Tetromino shapes (I, J, L, O, S, T, Z)
    static const int SHAPES[7][4][4];
    
    int currentPiece[PIECE_SIZE][PIECE_SIZE];
    int nextPiece[PIECE_SIZE][PIECE_SIZE];
    int currentType;
    int nextType;
    int posX;
    int posY;
    
    // Helper function to rotate a 4x4 matrix 90 degrees clockwise
    void rotateCW(int piece[PIECE_SIZE][PIECE_SIZE]);
    void copyPiece(const int source[PIECE_SIZE][PIECE_SIZE], 
                   int dest[PIECE_SIZE][PIECE_SIZE]) const;
    
public:
    // Constructor and Destructor
    Tetromino();
    ~Tetromino();
    
    // Initialization and spawning
    void initialize();
    void spawnNewPiece();
    
    // Position management
    void setPosition(int x, int y);
    void moveBy(int dx, int dy);
    int getPosX() const { return posX; }
    int getPosY() const { return posY; }
    
    // Rotation
    void rotate();
    
    // Piece data access
    int getCurrentType() const { return currentType; }
    int getNextType() const { return nextType; }
    const int* getCurrentPiece() const;
    const int* getNextPiece() const;
    int getPieceAt(int x, int y) const;
    
    // Type-specific piece data
    static const int* getPieceShape(int type);
};

#endif // TETROMINO_H
