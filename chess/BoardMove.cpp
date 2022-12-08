//
// Created by zbv11 on 07-Dec-22.
//
#include <functional>
#include "BoardMove.h"
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

Move ParseStdMove(Board board, char fromSquare, char toSquare) {
    char capture = board.GetPieceAt(toSquare);

    return {
        fromSquare,
        toSquare,
        capture
    };
}