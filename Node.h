#include "Board.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Node {
  private:
    int8_t generate_subtree_helper(uint8_t, uint8_t);
    void print_tree_helper(uint8_t, uint8_t);
    uint64_t get_tree_size_helper(uint64_t);


  public:
    Board board;
    uint8_t n_children;
    Node **children;
    int8_t branch_score;

    Node(Board);
    ~Node();
    void add_child(Node *);
    void create_next_states(uint8_t);
    void generate_subtree(uint8_t);
    void print_tree(uint8_t);
    uint64_t get_tree_size();
};
