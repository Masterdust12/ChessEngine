//
// Created by zbv11 on 07-Dec-22.
//

#ifndef CHESSENGINE_BOARDMOVE_H
#define CHESSENGINE_BOARDMOVE_H

#include <functional>
#include <iostream>
#include <memory>
#include "Square.h"

class Board;

struct Move {
    Square fromSquare, toSquare;
    char capturedPiece;
    char promotion = EMPTY_SQUARE;
    Square EPSquare = -1;

    bool operator==(const std::string& move) const;
    bool operator!=(const std::string& move) const;
    bool operator==(const Move& move) const;
    bool operator!=(const Move& move) const;

    bool Invalid() const;
};

/**
 * Converts a from and to square to a move struct.
 * This function assumes that:
 *   - The move is either a normal move or a capture.
 *   - The move is not a castling move, an en passant move, or a promotion move.
 */
Move ParseStdMove(const Board& board, const Square& fromSquare, const Square& toSquare);

/**
 * Converts a from and to square to a move struct.
 * This function assumes that:
 *   - The move is either a normal move or a capture.
 *   - The move is not a castling move, an en passant move, or a promotion move.
 */
Move ParseStdMove(const Board& board, Square fromSquare, int8_t fileOff, int8_t rankOff);

/**
 *
 */
void AddPawnMoves(const Board& board, std::list<Move>& moveList, const Square& fromSquare, const Square& toSquare);

#endif //CHESSENGINE_BOARDMOVE_H
