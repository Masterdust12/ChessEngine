//
// Created by zbv11 on 07-Dec-22.
//

#ifndef CHESSENGINE_BOARDMOVE_H
#define CHESSENGINE_BOARDMOVE_H

#include <functional>
#include <iostream>
#include "Square.h"

class Board;

struct Move {
    Square *fromSquare, *toSquare;
    char capturedPiece;
    std::function<void()> *OnMove = nullptr, *OnUndo = nullptr;
    int8_t EPSquare = -1;

    bool operator==(const std::string& move) const;
    bool operator!=(const std::string& move) const;
    bool operator==(const Move& move) const;
    bool operator!=(const Move& move) const;

    bool Invalid() const;

    ~Move() {
        std::cout << "Move Destructor" << std::endl;
        std::cout << "fromSquare: " << (std::string) *fromSquare << std::endl;
        std::cout << "toSquare: " << (std::string) *toSquare << std::endl;

        delete OnMove;
        delete OnUndo;
        delete fromSquare;
        delete toSquare;
    }
};

/**
 * Converts a from and to square to a move struct.
 * This function assumes that:
 *   - The move is either a normal move or a capture.
 *   - The move is not a castling move, an en passant move, or a promotion move.
 */
Move ParseStdMove(const Board& board, Square fromSquare, Square toSquare);

/**
 * Converts a from and to square to a move struct.
 * This function assumes that:
 *   - The move is either a normal move or a capture.
 *   - The move is not a castling move, an en passant move, or a promotion move.
 */
Move ParseStdMove(const Board& board, Square fromSquare, int8_t fileOff, int8_t rankOff);

#endif //CHESSENGINE_BOARDMOVE_H
