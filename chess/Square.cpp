//
// Created by zbv11 on 13-Dec-22.
//

#include "Square.h"
#include "Board.h"

Square::Square(const Square &square) {
    file = square.file;
    rank = square.rank;
}

Square::Square(int8_t index) {
    file = index % BOARD_WIDTH;
    rank = 7 - index / BOARD_WIDTH;
}

Square::Square(int8_t file, int8_t rank) {
    this->file = file;
    this->rank = rank;
}

Square::Square(int8_t index, int8_t fileOffset, int8_t rankOffset) {
    file = (7 - index % BOARD_WIDTH) + fileOffset;
    rank = (7 - index / BOARD_WIDTH) + rankOffset;
}

Square::Square(Square base, int8_t fileOffset, int8_t rankOffset) {
    file = base.file + fileOffset;
    rank = base.rank + rankOffset;
}

Square::operator int8_t() const {
    if (rank < 0 || rank > 7 || file < 0 || file > 7)
        return -1;

    return (7 - rank) * 8 + file;
}

Square::operator std::string() const {
    return std::string(1, 'a' + file) + std::to_string(rank + 1);
}

bool Square::Valid() const {
    return rank >= 0 && rank < 8 && file >= 0 && file < 8;
}

Square Square::operator +(const Square& move) const {
    return {static_cast<int8_t>(file + move.file), static_cast<int8_t>(rank + move.rank)};
}

Square Square::operator +=(const Square& move) {
    file += move.file;
    rank += move.rank;
    return *this;
}