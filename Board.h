#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Board {
  private:
  public:
    uint16_t board[8];
    int8_t score;

    Board();
    Board(uint16_t *);
    ~Board();
    void print_board();
    void get_unpacked_board(uint8_t **);
    bool check_valid_move(uint8_t, uint8_t, uint8_t);
    void apply_move(uint8_t, uint8_t, uint8_t);
    uint8_t get_current_player();
};
