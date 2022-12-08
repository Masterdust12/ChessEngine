//
// Created by zbv11 on 07-Dec-22.
//

#ifndef CHESSENGINE_BOARDMOVE_H
#define CHESSENGINE_BOARDMOVE_H

#include <functional>

class Board;

struct Move {
    char fromSquare, toSquare, capturedPiece;
    std::function<void()> *OnMove = nullptr, *OnUndo = nullptr;

    bool operator==(const std::string& move) const;
    bool operator!=(const std::string& move) const;
    bool operator==(const Move& move) const;
    bool operator!=(const Move& move) const;

    bool Invalid() const;

    ~Move() {
        delete OnMove;
        delete OnUndo;
    }
};

/**
 * Converts a from and to square to a move struct.
 * This function assumes that:
 *   - The move is either a normal move or a capture.
 *   - The move is not a castling move, an en passant move, or a promotion move.
 */
Move ParseStdMove(Board board, char fromSquare, char toSquare);

#endif //CHESSENGINE_BOARDMOVE_H
