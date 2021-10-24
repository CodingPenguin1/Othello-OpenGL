#include "Node.h"
#include <stdint.h>
#include <stdio.h>


// int main(int argc, char **argv) {
int main() {
    uint16_t *initial_board = new uint16_t[8];
    initial_board[3] = 0x0240;  // 1 is black and 2 is white
    initial_board[4] = 0x0180;

    // print_board(initial_board);

    Node *root = new Node(initial_board);
    root->create_next_states(1);
    printf("%d children\n", root->n_children);
    printf("Original\n");
    root->print_board();

    for (int i = 0; i < root->n_children; i++) {
        printf("\nChild %d\n", i);
        root->children[i]->print_board();
    }

    printf("\n\n\n--------\n\n\n");

    for (int i = 0; i < root->n_children; i++) {
        Node *sub_root = root->children[i];

        sub_root->create_next_states(2);
        printf("\n\n%d children\n", sub_root->n_children);
        printf("Original\n");
        sub_root->print_board();

        for (int i = 0; i < sub_root->n_children; i++) {
            printf("\nChild %d\n", i);
            sub_root->children[i]->print_board();
        }
    }


    return 0;
}
