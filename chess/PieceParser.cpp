//
// Created by zbv11 on 07-Dec-22.
//

#include "PieceParser.h"
#include "Board.h"

void ParsePawn(Board &board, const Square &index) {
    const int8_t side = (board.turn) ? 1 : -1;
    bool startRank = index.rank == (board.turn ? 1 : 6);

    // Capture Left
    if (board.Enemy(index, -1, side))
        board.AddPseudoLegalMove(ParseStdMove(board, index, -1, side));

    // Capture Right
    if (board.Enemy(index, 1, side))
        board.AddPseudoLegalMove(ParseStdMove(board, index, 1, side));

    // Forward (1 Square)
    if (board.Empty(index, 0, side)) {
        board.AddPseudoLegalMove(ParseStdMove(board, index, 0, side));

        // Forward (2 Squares)
        if (startRank && board.Empty(index, 0, 2 * side)) {
            Move move = ParseStdMove(board, index, 0, 2 * side);
            move.EPSquare = move.toSquare + Square(0, side);

            board.AddPseudoLegalMove(move);
        }
    }

    Square enPassant = board.GetEnPassantSquare();

    if (enPassant.Valid()) {
        const Square difference = index - enPassant;

        if (abs(difference.file) == 1 && difference.rank == side) {
            board.AddPseudoLegalMove(ParseStdMove(board, index, -difference.file, difference.rank));
        }
    }
}

void ParseBishop(Board &board, const Square &index) {
    const Square directions[] = {
            {1, 1}, {-1, -1}, {-1, 1}, {1, -1}
    };

    for (const Square& dir : directions) {
        Square origin = index;

        while (origin.Valid()) {
            origin += dir;

            if (board.Ally(origin)) break;

            board.AddPseudoLegalMove(ParseStdMove(board, index, origin));

            if (!board.Empty(origin)) break;
        }
    }
}

void ParseKnight(Board &board, const Square &index) {
    board.AddPseudoLegalMove(ParseStdMove(board, index, 1, 2));
    board.AddPseudoLegalMove(ParseStdMove(board, index, -1, 2));
    board.AddPseudoLegalMove(ParseStdMove(board, index, 1, -2));
    board.AddPseudoLegalMove(ParseStdMove(board, index, -1, -2));
    board.AddPseudoLegalMove(ParseStdMove(board, index, 2, 1));
    board.AddPseudoLegalMove(ParseStdMove(board, index, 2, -1));
    board.AddPseudoLegalMove(ParseStdMove(board, index, -2, 1));
    board.AddPseudoLegalMove(ParseStdMove(board, index, -2, -1));
}

void ParseRook(Board &board, const Square &index) {
    const Square directions[] = {
            {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    for (const Square& dir : directions) {
        Square origin = index;

        while (origin.Valid()) {
            origin += dir;

            if (board.Ally(origin)) break;

            board.AddPseudoLegalMove(ParseStdMove(board, index, origin));

            if (!board.Empty(origin)) break;
        }
    }
}

void ParseQueen(Board &board, const Square &index) {
    ParseRook(board, index);
    ParseBishop(board, index);
}

void ParseKing(Board &board, const Square &index) {
    for (int8_t i = -1; i <= 1; i++) {
        for (int8_t j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;

            board.AddPseudoLegalMove(ParseStdMove(board, index, i, j));
        }
    }
}
