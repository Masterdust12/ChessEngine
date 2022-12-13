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
            move.EPSquare = *move.toSquare - 8 * side;

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
    int xdelta = index % 8 - 1;
    int ydelta = index / 8;

    const int directions[] = {
            -9, -7, 7, 9
    };
    const int bounds[] = {
            index - 9 * std::min(7 - xdelta, ydelta), index - 7 * std::min(xdelta, ydelta),
            index + 7 * std::min(7 - xdelta, 7 - ydelta), index + 9 * std::min(xdelta, 7 - ydelta)
    };

    for (int i = 0; i < 4; i++) {
        int8_t off = directions[i];
        int8_t bound = bounds[i];

        for (int8_t dI = index + off; SidedCompare(dI, bound, off < 0) || dI == bound; dI += off) {
            if (board.Ally(dI))
                break;

            board.AddPseudoLegalMove(ParseStdMove(board, index, dI));

            if (!board.Empty(dI))
                break;
        }
    }
}

void ParseKnight(Board &board, int8_t index) {
    // Frankly, just a really stupid way to generate possible knight moves
    const int8_t offsets[] = {
            1, -1, 2, -2
    };

    for (int i = 0; i < 4; i++) {
        board.AddPseudoLegalMove(ParseStdMove(board, index, offsets[i], offsets[(i <= 2) * 2]));
        board.AddPseudoLegalMove(ParseStdMove(board, index, offsets[i], offsets[(i <= 2) * 2 + 1]));
    }
}

void ParseRook(Board &board, int8_t index) {
    int8_t delta = index % 8;

    const int directions[] = {
            1, -1, 8, -8
    };
    const int bounds[] = {
            index + 7 - delta, index - delta, 64 - delta, 7 - delta
    };

    for (int i = 0; i < 4; i++) {
        int off = directions[i];

        for (int8_t dI = index + off; dI != bounds[i]; dI += off) {
            if (board.Ally(dI))
                break;

            board.AddPseudoLegalMove(ParseStdMove(board, index, dI));

            if (!board.Empty(dI))
                break;
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
