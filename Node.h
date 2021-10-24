#include <stdint.h>

class Node {
  private:
  public:
    uint16_t board[8];
    uint8_t n_children;
    Node **children;
    int8_t score;
    float branch_score;

    Node(const uint16_t *);
    ~Node();
    void print_board();
    void add_child(Node *);
    void create_next_states(uint8_t);
    uint8_t **get_valid_moves();
    uint8_t **get_unpacked_board();
    bool check_valid_move(uint8_t, uint8_t, uint8_t);
    bool apply_move(uint8_t, uint8_t, uint8_t);
};
