#include "Node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node::Node(const uint16_t *&board) {
    memcpy(this->board, board, sizeof(uint16_t) * 8);
    this->n_children = 0;
    this->children = NULL;
    this->score = 0;
    this->branch_score = 0.0f;
}

Node::~Node() {
    if (this->children != NULL)
        delete[] this->children;
}
