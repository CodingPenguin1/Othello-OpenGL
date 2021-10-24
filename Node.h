#include "Board.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Node {
  private:
    void generate_subtree_helper(uint8_t, uint8_t, Node *);
    void print_tree_helper(uint8_t, uint8_t, Node *);
    uint64_t get_tree_size_helper(uint64_t, Node *);


  public:
    Board board;
    uint8_t n_children;
    Node **children;
    float branch_score;

    Node(Board);
    ~Node();
    void print_board();
    void add_child(Node *);
    void create_next_states(uint8_t);
    void generate_subtree(uint8_t);
    void print_tree(uint8_t);
    uint64_t get_tree_size();
};
