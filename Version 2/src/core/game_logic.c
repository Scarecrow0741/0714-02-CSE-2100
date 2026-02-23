#include "../include/tetris.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int TETROMINOS[7][4][4] = {
    // I
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    // J
    {
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // L
    {
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // O
    {
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // S
    {
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // T
    {
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    },
    // Z
    {
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0},
        {0,0,0,0}
    }
};

void init_game(GameState *game) {
    for (int y=0; y<BOARD_HEIGHT; y++)
        for (int x=0; x<BOARD_WIDTH; x++)
            game->board[y][x] = EMPTY_CELL;

    game->score = 0;
    game->game_over = 0;
    game->piece_x = (BOARD_WIDTH/2) - 2;
    game->piece_y = 0;

    srand((unsigned)time(NULL));
    game->next_type = rand() % 7;
    spawn_tetromino(game);
}

void spawn_tetromino(GameState *game) {
    game->current_type = game->next_type;
    game->next_type = rand() % 7;

    for (int y=0; y<4; y++)
        for (int x=0; x<4; x++) {
            game->current_piece[y][x] = TETROMINOS[game->current_type][y][x];
            game->next_piece[y][x] = TETROMINOS[game->next_type][y][x];
        }

    game->piece_x = (BOARD_WIDTH/2) - 2;
    game->piece_y = 0;

    if (check_collision(game)) {
        game->game_over = 1;
    }
}

int check_collision(const GameState *game) {
    for (int y=0; y<4; y++) {
        for (int x=0; x<4; x++) {
            if (game->current_piece[y][x]) {
                int board_x = game->piece_x + x;
                int board_y = game->piece_y + y;

                if (board_x < 0 || board_x >= BOARD_WIDTH ||
                    board_y >= BOARD_HEIGHT ||
                    (board_y >= 0 && game->board[board_y][board_x])) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void rotate_piece(GameState *game) {
    int rotated[4][4] = {0};
    for (int y=0; y<4; y++)
        for (int x=0; x<4; x++)
            rotated[x][3 - y] = game->current_piece[y][x];

    int original[4][4];
    for (int y=0; y<4; y++)
        for (int x=0; x<4; x++) {
            original[y][x] = game->current_piece[y][x];
            game->current_piece[y][x] = rotated[y][x];
        }

    if (check_collision(game)) {
        game->piece_x -= 1;
        if (check_collision(game)) {
            game->piece_x += 2;
            if (check_collision(game)) {
                // revert
                game->piece_x -= 1;
                for (int y=0; y<4; y++)
                    for (int x=0; x<4; x++)
                        game->current_piece[y][x] = original[y][x];
            }
        }
    }
}

void move_piece(GameState *game, int dx, int dy) {
    game->piece_x += dx;
    game->piece_y += dy;

    if (check_collision(game)) {
        game->piece_x -= dx;
        game->piece_y -= dy;

        if (dy > 0) {
            // Lock piece in place
            for (int y=0; y<4; y++) {
                for (int x=0; x<4; x++) {
                    if (game->current_piece[y][x]) {
                        int board_x = game->piece_x + x;
                        int board_y = game->piece_y + y;
                        if (board_y >= 0 && board_x >= 0 && board_x < BOARD_WIDTH && board_y < BOARD_HEIGHT) {
                            game->board[board_y][board_x] = game->current_type + 1;
                        }
                    }
                }
            }
            clear_lines(game);
            spawn_tetromino(game);
        }
    }
}

void clear_lines(GameState *game) {
    int lines_cleared = 0;

    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        int line_complete = 1;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!game->board[y][x]) {
                line_complete = 0;
                break;
            }
        }
        if (line_complete) {
            lines_cleared++;
            for (int y2 = y; y2 > 0; y2--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    game->board[y2][x] = game->board[y2 - 1][x];
                }
            }
            for (int x = 0; x < BOARD_WIDTH; x++) {
                game->board[0][x] = EMPTY_CELL;
            }
            y++;
        }
    }

    if (lines_cleared > 0) {
        game->score += lines_cleared * lines_cleared * 100;
    }
}
