//
// Created by zbv11 on 07-Dec-22.
//
#include <functional>
#include "Board.h"

bool Move::operator==(const std::string& move) const {
    return fromSquare == Board::Index(move.substr(0, 2))
           && toSquare == Board::Index(move.substr(2, 2));
}

bool Move::operator!=(const std::string& move) const {
    return !(*this == move);
}

bool Move::operator==(const Move& move) const {
    return fromSquare == move.fromSquare
           && toSquare == move.toSquare;
}

bool Move::operator!=(const Move& move) const {
    return !(*this == move);
}

bool Move::Invalid() const {
    return fromSquare == -1 || toSquare == -1;
}

Move ParseStdMove(const Board& board, const Square& fromSquare, const Square& toSquare) {
    char capture = board.GetPieceAt(toSquare);

    return {fromSquare, toSquare, capture};
}

Move ParseStdMove(const Board& board, Square fromSquare, int8_t fileOff, int8_t rankOff) {
    return ParseStdMove(board, fromSquare, Square(fromSquare, fileOff, rankOff));
}