//
// Created by zbv11 on 13-Dec-22.
//

#include "Square.h"
#include "Board.h"

Square::Square() {
    file = -1;
    rank = -1;
}

Square::Square(const Square &square) {
    file = square.file;
    rank = square.rank;
}

Square::Square(const std::string &square) {
    file = square[0] - 'a';
    rank = square[1] - '1';
}

Square::Square(int8_t index) {
    if (index == -1) {
        file = -1;
        rank = -1;
        return;
    }

    file = index % BOARD_DIM;
    rank = 7 - index / BOARD_DIM;
}

Square::Square(int8_t file, int8_t rank) {
    this->file = file;
    this->rank = rank;
}

Square::Square(int8_t index, int8_t fileOffset, int8_t rankOffset) {
    file = (7 - index % BOARD_DIM) + fileOffset;
    rank = (7 - index / BOARD_DIM) + rankOffset;
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

Square Square::operator -(const Square& move) const {
    return {static_cast<int8_t>(file - move.file), static_cast<int8_t>(rank - move.rank)};
}

Square Square::operator +=(const Square& move) {
    file += move.file;
    rank += move.rank;
    return *this;
}