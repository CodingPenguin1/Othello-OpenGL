#include "Node.h"
#include <stdint.h>
#include <stdio.h>


void print_board(uint16_t *board) {
    for (uint8_t row = 0; row < 8; ++row) {
        for (uint8_t col = 0; col < 8; ++col) {
            uint16_t current_space = (board[row] >> 2 * (8 - col - 1)) & 0x3;  // Shift to the right so that current cell is right most, 0 padded on left
            switch (current_space) {
                case 0:  // Empty
                    printf(".");
                    break;
                case 1:  // Black
                    printf("X");
                    break;
                case 2:  // White
                    printf("O");
                    break;
                default:  // Error
                    printf("?");
                    break;
            }
        }
        printf("\n");
    }
}


// int main(int argc, char **argv) {
int main() {
    uint16_t *initial_board = new uint16_t[8];
    initial_board[3] = 0x0240;  // 1 is black and 2 is white
    initial_board[4] = 0x0180;

    print_board(initial_board);

    return 0;
}
