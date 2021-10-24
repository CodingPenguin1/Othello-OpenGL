#include "Board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Board::Board() {
    for (int row = 0; row < 8; ++row) {
        this->board[row] = 0x0000;
    }
}


Board::Board(uint16_t *board_data) {
    memcpy(this->board, board_data, sizeof(uint16_t) * 8);
    this->score = 0;
}

Board::~Board() {
}


void Board::print_board() {
    for (uint8_t row = 0; row < 8; ++row) {
        for (uint8_t col = 0; col < 8; ++col) {
            uint16_t current_space = (this->board[row] >> 2 * (8 - col - 1)) & 0x3;  // Shift to the right so that current cell is right most, 0 padded on left
            switch (current_space) {
                case 0:  // Empty
                    printf(".");
                    break;
                case 1:  // Black
                    printf("X");
                    break;
                case 2:  // White
                    printf("O");
                    break;
                default:  // Error
                    printf("?");
                    break;
            }
        }
        printf("\n");
    }
}


bool Board::check_valid_move(uint8_t row, uint8_t col, uint8_t player) {
    uint8_t **unpacked_board = this->get_unpacked_board();

    // If space is not empty
    if (unpacked_board[row][col] != 0)
        return false;

    // Iterate through each direction
    for (int d_row = -1; d_row < 2; ++d_row) {
        for (int d_col = -1; d_col < 2; ++d_col) {
            int cur_row = row;
            int cur_col = col;
            // If direction is not 0,0
            if (d_row != 0 || d_col != 0) {
                // Iterate in that direction
                cur_row += d_row;
                cur_col += d_col;

                bool found_enemy = false;
                bool found_self = false;

                while (cur_row > -1 && cur_row < 8 && cur_col > -1 && cur_col < 8) {
                    if (unpacked_board[cur_row][cur_col] == player)
                        found_self = true;
                    if (unpacked_board[cur_row][cur_col] > 0 && unpacked_board[cur_row][cur_col] != player)
                        found_enemy = true;

                    // If found self before enemy, not valid move
                    if (found_self && !found_enemy)
                        break;

                    // If gap found, not valid move
                    if (unpacked_board[cur_row][cur_col] == 0)
                        break;

                    if (found_enemy && found_self)
                        return true;

                    cur_row += d_row;
                    cur_col += d_col;
                }
            }
        }
    }
    return false;
}


bool Board::apply_move(uint8_t row, uint8_t col, uint8_t player) {
    uint8_t **unpacked_board = this->get_unpacked_board();

    // Iterate through each direction
    for (int d_row = -1; d_row < 2; ++d_row) {
        for (int d_col = -1; d_col < 2; ++d_col) {
            int cur_row = row;
            int cur_col = col;
            bool valid_direction = false;
            // If direction is not 0,0
            if (d_row != 0 || d_col != 0) {
                // Iterate in that direction
                cur_row += d_row;
                cur_col += d_col;

                bool found_enemy = false;
                bool found_self = false;

                while (cur_row > -1 && cur_row < 8 && cur_col > -1 && cur_col < 8) {
                    if (unpacked_board[cur_row][cur_col] == player)
                        found_self = true;
                    if (unpacked_board[cur_row][cur_col] > 0 && unpacked_board[cur_row][cur_col] != player)
                        found_enemy = true;

                    // If found self before enemy, not valid move
                    if (found_self && !found_enemy)
                        break;

                    // If gap found, not valid move
                    if (unpacked_board[cur_row][cur_col] == 0)
                        break;

                    if (found_enemy && found_self) {
                        valid_direction = true;
                        break;
                    }

                    cur_row += d_row;
                    cur_col += d_col;
                }
            }

            if (valid_direction) {
                cur_row = row;
                cur_col = col;

                while (cur_row > -1 && cur_row < 8 && cur_col > -1 && cur_col < 8 && unpacked_board[cur_row][cur_col] != player) {
                    unpacked_board[cur_row][cur_col] = player;
                    cur_row += d_row;
                    cur_col += d_col;
                }
            }
        }
    }

    // Repack board
    for (int row = 0; row < 8; ++row) {
        this->board[row] = 0x0000;
        for (int col = 0; col < 8; ++col) {
            this->board[row] |= (unpacked_board[row][col] << 2 * (8 - col - 1));
        }
    }
}


uint8_t **Board::get_unpacked_board() {
    uint8_t **unpacked_board = new uint8_t *[8];
    for (int row = 0; row < 8; ++row) {
        unpacked_board[row] = new uint8_t[8];
        for (uint8_t col = 0; col < 8; ++col)
            unpacked_board[row][col] = (this->board[row] >> 2 * (8 - col - 1)) & 0x3;
    }
    return unpacked_board;
}