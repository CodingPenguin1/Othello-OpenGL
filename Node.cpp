#include "Node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node::Node(const uint16_t *board) {
    memcpy(this->board, board, sizeof(uint16_t) * 8);
    this->n_children = 0;
    this->children = new Node *[0];
    this->score = 0;
    this->branch_score = 0.0f;
}

Node::~Node() {
    delete[] this->children;
}


void Node::print_board() {
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


void Node::add_child(Node *child) {
    this->n_children++;
    Node **temp_children_array = new Node *[this->n_children];
    for (uint8_t i = 0; i < this->n_children - 1; ++i)
        temp_children_array[i] = this->children[i];
    temp_children_array[this->n_children - 1] = child;
    delete[] this->children;
    this->children = temp_children_array;
}


void Node::create_next_states(uint8_t player) {
    // Check all spaces for potential moves
    for (uint8_t row = 0; row < 8; ++row) {
        for (uint8_t col = 0; col < 8; ++col) {
            if (this->check_valid_move(row, col, player)) {
                // Create a new child with the move applied
                Node *child = new Node(this->board);
                child->apply_move(row, col, player);
                this->add_child(child);
            }
        }
    }
}


bool Node::check_valid_move(uint8_t row, uint8_t col, uint8_t player) {
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


bool Node::apply_move(uint8_t row, uint8_t col, uint8_t player) {
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


uint8_t **Node::get_unpacked_board() {
    uint8_t **unpacked_board = new uint8_t *[8];
    for (int row = 0; row < 8; ++row) {
        unpacked_board[row] = new uint8_t[8];
        for (uint8_t col = 0; col < 8; ++col)
            unpacked_board[row][col] = (this->board[row] >> 2 * (8 - col - 1)) & 0x3;
    }
    return unpacked_board;
}
