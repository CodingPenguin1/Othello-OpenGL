#include "Board.h"
#include <stdint.h>

class Node {
  private:
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
};
