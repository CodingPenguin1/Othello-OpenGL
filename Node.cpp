#include "Node.h"


Node::Node(Board board) {
    this->board = board;
    this->n_children = 0;
    this->children = new Node *[0];
    this->branch_score = 0.0f;
}


Node::~Node() {
    for (int i = 0; i < this->n_children; ++i) {
        delete this->children[i];
    }
    delete[] this->children;
}


void Node::print_board() {
    board.print_board();
}


void Node::generate_subtree(uint8_t depth) {
    generate_subtree_helper(depth, 0, this);
}


void Node::generate_subtree_helper(uint8_t depth, uint8_t current_depth, Node *node) {
    // If we have not reached the desired depth, generate the next level of children
    if (current_depth < depth) {
        // Generate the children of the current node
        node->create_next_states(node->board.get_current_player());

        // Recurse on each child
        for (uint8_t i = 0; i < node->n_children; ++i) {
            generate_subtree_helper(depth, current_depth + 1, node->children[i]);
        }
    }
}


void Node::create_next_states(uint8_t player) {
// Check all spaces for potential moves
#pragma omp parallel for
    for (uint8_t row = 0; row < 8; ++row) {
        for (uint8_t col = 0; col < 8; ++col) {
            if (board.check_valid_move(row, col, player)) {
                // Create a new child with the move applied
                Node *child = new Node(board);
                child->board.apply_move(row, col, player);
#pragma omp critical
                {
                    this->add_child(child);
                }
            }
        }
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


void Node::print_tree(uint8_t depth) {
    print_tree_helper(depth, 0, this);
}


void Node::print_tree_helper(uint8_t depth, uint8_t current_depth, Node *node) {
    if (current_depth < depth && node->n_children > 0) {
        // Print node's board
        printf("\n\n\n--------\n\n\nDepth %d\n%d Children\nOriginal\n", current_depth, node->n_children);
        node->print_board();

        // Print children
        for (int i = 0; i < node->n_children; i++) {
            printf("\nChild %d\n", i);
            node->children[i]->print_board();
        }

        // Recurse on children
        for (int i = 0; i < node->n_children; i++) {
            print_tree_helper(depth, current_depth + 1, node->children[i]);
        }
    }
}


uint64_t Node::get_tree_size() {
    return get_tree_size_helper(0, this);
}


uint64_t Node::get_tree_size_helper(uint64_t current_size, Node *node) {
    current_size += sizeof(this);
    for (int i = 0; i < node->n_children; i++) {
        current_size += get_tree_size_helper(0, node->children[i]);
    }
    return current_size;
}