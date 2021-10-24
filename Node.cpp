#include "Node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node::Node(Board board) {
    this->board = board;
    this->n_children = 0;
    this->children = new Node *[0];
    this->branch_score = 0.0f;
}

Node::~Node() {
    delete[] this->children;
}


void Node::print_board() {
    board.print_board();
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
            if (board.check_valid_move(row, col, player)) {
                // Create a new child with the move applied
                Node *child = new Node(board);
                child->board.apply_move(row, col, player);
                this->add_child(child);
            }
        }
    }
}
