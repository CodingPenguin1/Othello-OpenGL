#include "Board.h"
#include <stdexcept>
#include <string>

Board::Board(int size) {
    // If board is not even size >= 4x4, throw exception
    if (size < 4 || size % 2)
        throw std::invalid_argument("Invalid board size");

    // Create empty array
    this->size = size;
    this->array = new int *[size];
    for (int i = 0; i < size; ++i)
        this->array[i] = new int[size];
    for (int row = 0; row < size; ++row)
        for (int col = 0; col < size; ++col)
            this->array[row][col] = 0;

    // Set starting pieces
    // -1 is black, 1 is white
    array[size / 2 - 1][size / 2 - 1] = 1;
    array[size / 2][size / 2 - 1] = -1;
    array[size / 2 - 1][size / 2] = -1;
    array[size / 2][size / 2] = 1;
}


Board::~Board() {
}


int *&Board::operator[](int i) {
    if (i >= this->size) throw std::invalid_argument("Board access index too big (" + std::to_string(i) + "), expected less than " + std::to_string(this->size));
    if (i < 0) throw std::invalid_argument("Board access index less than 0 (" + std::to_string(i) + ")");
    return this->array[i];
}


void Board::print() {
    for (int i = 0; i < this->size; ++i) {
        printf("%d %d %d %d %d %d %d %d\n", (*this)[i][0], (*this)[i][1], (*this)[i][2], (*this)[i][3], (*this)[i][4], (*this)[i][5], (*this)[i][6], (*this)[i][7]);
    }
}