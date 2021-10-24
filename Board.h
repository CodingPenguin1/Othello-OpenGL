#include <stdint.h>

class Board {
  private:
  public:
    uint16_t board[8];
    int8_t score;

    Board();
    Board(uint16_t *);
    ~Board();
    void print_board();
    uint8_t **get_unpacked_board();
    bool check_valid_move(uint8_t, uint8_t, uint8_t);
    bool apply_move(uint8_t, uint8_t, uint8_t);
};
