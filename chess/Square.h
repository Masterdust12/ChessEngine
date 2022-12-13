//
// Created by zbv11 on 13-Dec-22.
//

#ifndef CHESSENGINE_SQUARE_H
#define CHESSENGINE_SQUARE_H

#include <cstdint>
#include <string>

struct Square {
    int8_t file, rank;

    Square(int8_t index);
    Square(int8_t file, int8_t rank);
    Square(int8_t index, int8_t fileOffset, int8_t rankOffset);

    operator int8_t() const;
    operator std::string() const;
    Square operator +(const Square& move) const;
};

#endif //CHESSENGINE_SQUARE_H
