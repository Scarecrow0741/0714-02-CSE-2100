#ifndef TETRIS_H
#define TETRIS_H

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Game states
typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_GAME_OVER
} GameScreenState;

typedef enum {
    MENU_PLAY,
    MENU_EXIT
} MenuOption;

#define BOARD_WIDTH 20
#define BOARD_HEIGHT 20
#define EMPTY_CELL 0

#define WINDOW_BORDER 50
#define BLOCK_SIZE 20
#define NEXT_PREVIEW_SIZE 4
#define HUD_WIDTH 200

// Game state structure
typedef struct {
    int board[BOARD_HEIGHT][BOARD_WIDTH];
    int current_piece[4][4];
    int next_piece[4][4];
    int piece_x, piece_y;
    int current_type;
    int next_type;
    int score;
    int game_over;
    GameScreenState screen_state;
    MenuOption selected_option;

    // SDL
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    int win_w, win_h;
} GameState;

// Game logic functions
void init_game(GameState *game);
void spawn_tetromino(GameState *game);
int check_collision(const GameState *game);
void rotate_piece(GameState *game);
void move_piece(GameState *game, int dx, int dy);
void clear_lines(GameState *game);

// Rendering functions
void draw_game_sdl(GameState *game);
void draw_menu(SDL_Renderer* renderer, TTF_Font* font, MenuOption selected);
void draw_game_over(SDL_Renderer* renderer, TTF_Font* font, int final_score);
MenuOption check_menu_click(int x, int y);

// Tetromino shapes
extern const int TETROMINOS[7][4][4];

#endif // TETRIS_H
