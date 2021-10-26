#include "Board.h"


Board::Board() {
    for (int row = 0; row < 8; ++row)
        this->board[row] = 0x0000;
    this->score = get_score();
}


Board::Board(uint16_t *board_data) {
    memcpy(this->board, board_data, sizeof(uint16_t) * 8);
    this->score = get_score();
}


Board::~Board() {
}


void Board::print_board() {
    for (uint8_t row = 0; row < 8; ++row) {
        for (uint8_t col = 0; col < 8; ++col) {
            // Shift to the right so that current cell is right most, 0 padded on left
            uint16_t current_space = (this->board[row] >> 2 * (8 - col - 1)) & 0x3;
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


void Board::print_detailed_board() {
    // Prints board with metadata
    if (score > 0)
        printf("Score: %d (black)\n", score);
    else if (score < 0)
        printf("Score: %d (white)\n", score);
    else
        printf("Score: 0\n");

    uint8_t player = get_current_player();
    if (player == 1)
        printf("Turn: X\n");
    else
        printf("Turn: O\n");
    print_board();
}


int8_t Board::get_score() {
    int8_t _score = 0;
    for (uint8_t row = 0; row < 8; ++row) {
        for (uint8_t col = 0; col < 8; ++col) {
            // + 1 to score if black has a move
            if (check_valid_move(row, col, 1))
                _score += 1;
            // - 1 to score if white has a move
            else if (check_valid_move(row, col, 2))
                _score -= 1;
        }
    }
    return _score;
}


bool Board::check_valid_move(uint8_t row, uint8_t col, uint8_t player) {
    uint8_t **unpacked_board = new uint8_t *[8];
    for (int i = 0; i < 8; ++i)
        unpacked_board[i] = new uint8_t[8];
    this->get_unpacked_board(unpacked_board);

    // If space is not empty
    if (unpacked_board[row][col] != 0) {
        // Delete the unpacked board
        for (int i = 0; i < 8; ++i)
            delete[] unpacked_board[i];
        delete[] unpacked_board;
        return false;
    }

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

                    if (found_enemy && found_self) {
                        // Delete the unpacked board
                        for (int i = 0; i < 8; ++i)
                            delete[] unpacked_board[i];
                        delete[] unpacked_board;
                        return true;
                    }

                    cur_row += d_row;
                    cur_col += d_col;
                }
            }
        }
    }

    // Delete the unpacked board
    for (int i = 0; i < 8; ++i)
        delete[] unpacked_board[i];
    delete[] unpacked_board;

    return false;
}


void Board::apply_move(uint8_t row, uint8_t col, uint8_t player) {
    uint8_t **unpacked_board = new uint8_t *[8];
    for (int i = 0; i < 8; ++i)
        unpacked_board[i] = new uint8_t[8];
    this->get_unpacked_board(unpacked_board);

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
                unpacked_board[cur_row][cur_col] = player;

                cur_row += d_row;
                cur_col += d_col;

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

    // Delete the unpacked board
    for (int i = 0; i < 8; ++i)
        delete[] unpacked_board[i];
    delete[] unpacked_board;

    // Update the score
    this->score = this->get_score();
}


void Board::get_unpacked_board(uint8_t **unpacked_board) {
    for (int row = 0; row < 8; ++row)
        for (uint8_t col = 0; col < 8; ++col)
            unpacked_board[row][col] = (this->board[row] >> 2 * (8 - col - 1)) & 0x3;
}


uint8_t Board::get_current_player() {
    uint8_t **unpacked_board = new uint8_t *[8];
    for (int i = 0; i < 8; ++i)
        unpacked_board[i] = new uint8_t[8];
    this->get_unpacked_board(unpacked_board);

    uint8_t num_pieces = 0;
    for (int row = 0; row < 8; ++row)
        for (int col = 0; col < 8; ++col)
            if (unpacked_board[row][col] > 0)
                ++num_pieces;

    // Delete the unpacked board
    for (int i = 0; i < 8; ++i)
        delete[] unpacked_board[i];
    delete[] unpacked_board;

    // If an even amount of pieces, player 1
    if (num_pieces % 2 == 0)
        return 1;
    return 2;
}
