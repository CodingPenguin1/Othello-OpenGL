#include "Node.h"
#include <stdint.h>
#include <stdio.h>


// int main(int argc, char **argv) {
int main() {
    uint16_t initial_board[8];
    initial_board[3] = 0x0240;  // 1 is black and 2 is white
    initial_board[4] = 0x0180;

    Node *root = new Node(Board(initial_board));
    uint8_t tree_depth = 8;
    root->generate_subtree(tree_depth);
    printf("done generating tree (depth %d)\n", tree_depth);

    uint64_t tree_size = root->get_tree_size();
    printf("%lu B\n", tree_size);
    printf("%f KB\n", tree_size / 1024.0);
    printf("%f MB\n", tree_size / (1024.0 * 1024.0));
    printf("%f GB\n", tree_size / (1024.0 * 1024.0 * 1024.0));

    // root->print_tree(5);

    delete root;

    return 0;
}
