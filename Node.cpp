#include "Node.h"
#include <math.h>


Node::Node(Board board) {
    this->board = board;
    this->n_children = 0;
    this->children = new Node *[0];
    this->branch_score = 0;
}


Node::~Node() {
    for (int i = 0; i < this->n_children; ++i) {
        delete this->children[i];
    }
    delete[] this->children;
}


void Node::generate_subtree(uint8_t depth) {
    this->branch_score = generate_subtree_helper(depth, 0);
}


int8_t Node::generate_subtree_helper(uint8_t depth, uint8_t current_depth) {
    // If we have not reached the desired depth, generate the next level of children
    if (current_depth < depth) {
        // Get whose turn it is
        uint8_t current_player = this->board.get_current_player();

        // Generate the children of the current node
        this->create_next_states(this->board.get_current_player());

        // Recurse on each child
        this->branch_score = current_player == 1 ? -128 : 127;
        for (uint8_t i = 0; i < this->n_children; ++i) {
            int8_t candidate_branch_score = this->children[i]->generate_subtree_helper(depth, current_depth + 1);
            if (current_player == 1 && candidate_branch_score > this->branch_score)
                this->branch_score = candidate_branch_score;
            else if (current_player == 2 && candidate_branch_score < this->branch_score)
                this->branch_score = candidate_branch_score;
        }

        // Return the branch score
        // printf("branch score: %d\n", this->branch_score);
        return this->branch_score;
    }

    // Return the normal score if we're at the bottom of the tree
    // printf("normal score: %d\n", node->board.get_score());
    return this->board.get_score();
}


void Node::create_next_states(uint8_t player) {
    // Check all spaces for potential moves
    // #pragma omp parallel for
    for (uint8_t row = 0; row < 8; ++row) {
        for (uint8_t col = 0; col < 8; ++col) {
            if (board.check_valid_move(row, col, player)) {
                // Create a new child with the move applied
                Node *child = new Node(board);
                child->board.apply_move(row, col, player);
                // #pragma omp critical
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
    print_tree_helper(depth, 0);
}


void Node::print_tree_helper(uint8_t depth, uint8_t current_depth) {
    if (current_depth<depth &&this->n_children> 0) {
        // Print node's board
        printf("\n\n\n--------\n\n\nDepth %d\n%d Children\n", current_depth, this->n_children);
        printf("\nOriginal\n");
        printf("Branch: %d\n", this->branch_score);
        this->board.print_detailed_board();

        // Print children
        for (int i = 0; i < this->n_children; i++) {
            printf("\nChild %d\n", i);
            this->children[i]->board.print_detailed_board();
        }

        // Recurse on children
        for (int i = 0; i < this->n_children; i++) {
            this->children[i]->print_tree_helper(depth, current_depth + 1);
        }
    }
}


uint64_t Node::get_tree_size() {
    return get_tree_size_helper(0);
}


uint64_t Node::get_tree_size_helper(uint64_t current_size) {
    current_size += sizeof(this);
    for (int i = 0; i < this->n_children; i++)
        current_size += this->children[i]->get_tree_size_helper(0);
    return current_size;
}
