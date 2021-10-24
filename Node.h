#include <stdint.h>

class Node {
    private:
    public:
        uint16_t board[8];
        uint8_t n_children;
        Node** children;
        int8_t score;
        float branch_score;

        Node(const uint16_t*&);
        ~Node();
};
