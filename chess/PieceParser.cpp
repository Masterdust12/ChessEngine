//
// Created by zbv11 on 07-Dec-22.
//

#include <functional>
#include "PieceParser.h"
#include "Board.h"

void ParsePawn(Board &board, int8_t index) {
    const int8_t side = (board.turn) ? 1 : -1;
    bool startRank = Board::Rank(index) == (board.turn ? 1 : 6);

    // Capture Left
    if (board.Enemy(index, -1, side) && Board::File(index) != 0)
        board.AddPseudoLegalMove(ParseStdMove(board, index, index + 7 * side));

    if (board.Enemy(index, 1, side) && Board::File(index) != 7)
        board.AddPseudoLegalMove(ParseStdMove(board, index, index + 9 * side));

    if (board.Empty(index, 0, side)) {
        board.AddPseudoLegalMove(ParseStdMove(board, index, index + 8 * side));

        if (!startRank || !board.Empty(index, 0, 2 * side)) {
            Move move = ParseStdMove(board, index, index + 16 * side);
            move.EPSquare = move.toSquare - 8 * side;

            board.AddPseudoLegalMove(move);
        }
    }

    int8_t enPassant = board.GetEnPassantSquare();

    if (enPassant != -1) {
        if (index - enPassant == 7 * side || index - enPassant == 9 * side) {
            Move move = ParseStdMove(board, index, enPassant);
            move.OnMove = new std::function<void()>([&] {
                board.SetPieceAt(enPassant,0);
            });
            move.OnUndo = new std::function<void()>([&] {
                board.SetPieceAt(enPassant, (board.turn) ? 'p' : 'P');
            });
            board.AddPseudoLegalMove(move);
        }
    }
}

void ParseBishop(Board &board, int8_t index) {
    Square square(index);

    const Square directions[] = {
            {1, 1}, {-1, -1}, {-1, 1}, {1, -1}
    };

    for (const Square& dir : directions) {
        Square origin = square;

        while (origin.Valid()) {
            origin += dir;

            if (board.Ally(origin)) break;

            board.AddPseudoLegalMove(ParseStdMove(board, square, origin));

            if (!board.Empty(origin)) break;
        }
    }
}

void ParseKnight(Board &board, int8_t index) {
    board.AddPseudoLegalMove(ParseStdMove(board, index, 1, 2));
    board.AddPseudoLegalMove(ParseStdMove(board, index, -1, 2));
    board.AddPseudoLegalMove(ParseStdMove(board, index, 1, -2));
    board.AddPseudoLegalMove(ParseStdMove(board, index, -1, -2));
    board.AddPseudoLegalMove(ParseStdMove(board, index, 2, 1));
    board.AddPseudoLegalMove(ParseStdMove(board, index, 2, -1));
    board.AddPseudoLegalMove(ParseStdMove(board, index, -2, 1));
    board.AddPseudoLegalMove(ParseStdMove(board, index, -2, -1));
}

void ParseRook(Board &board, int8_t index) {
    Square square(index);

    const Square directions[] = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    for (const Square& dir : directions) {
        Square origin = square;

        while (origin.Valid()) {
            origin += dir;

            if (board.Ally(origin)) break;

            board.AddPseudoLegalMove(ParseStdMove(board, square, origin));

            if (!board.Empty(origin)) break;
        }
    }
}

void ParseQueen(Board &board, int8_t index) {
    ParseRook(board, index);
    ParseBishop(board, index);
}

void ParseKing(Board &board, int8_t index) {
    for (int8_t i = -1; i <= 1; i++) {
        for (int8_t j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;

            board.AddPseudoLegalMove(ParseStdMove(board, index, i, j));
        }
    }
}
