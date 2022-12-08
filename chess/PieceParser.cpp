//
// Created by zbv11 on 07-Dec-22.
//

#include "PieceParser.h"

void ParsePawn(Board &board, char index) {
    const char side = (board.turn) ? 1 : -1;
    bool startRank = Board::Rank(index) == (board.turn ? 1 : 6);

    if (board.Enemy(index + 7 * side))
        board.AddPseudoLegalMove(ParseStdMove(board, index, index + 7 * side));

    if (board.Enemy(index + 9 * side))
        board.AddPseudoLegalMove(ParseStdMove(board, index, index + 9 * side));

    if (board.Empty(index + 8 * side)) {
        board.AddPseudoLegalMove(ParseStdMove(board, index, index + 8 * side));

        if (startRank && board.Empty(index + 16 * side)) {
            Move move = ParseStdMove(board, index, index + 16 * side);
            move.OnMove = new std::function<void()>([&] {
                board.AddEnPassantSquare(board.MoveNum(), index + 8 * side);
            });
        }
    }

    Square *enPassant = board.GetEnPassantSquare();

    if (enPassant) {
        if (enPassant->file == Board::File(index) - 1) {
            Move move = ParseStdMove(board, index, *enPassant + Square(-1, 0));
            move.OnMove = new std::function<void()>([&] {
                board.SetPieceAt(*enPassant, 0);
            });
            move.OnUndo = new std::function<void()>([&] {
                board.SetPieceAt(*enPassant, (board.turn) ? 'p' : 'P');
            });
        }
    }

    delete enPassant;
}

void ParseBishop(Board &board, char index) {
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
        char off = directions[i];
        char bound = bounds[i];

        for (char dI = index + off; SidedCompare(dI, bound, off < 0) || dI == bound; dI += off) {
            if (board.Ally(dI))
                break;

            board.AddPseudoLegalMove(ParseStdMove(board, index, dI));

            if (!board.Empty(dI))
                break;
        }
    }
}

void ParseKnight(Board &board, char index) {
    Square fromSquare = index;

    const char offsets[] = {
            1, -1, 2, -2
    };

    // Frankly, just a really stupid way to generate possible knight moves;
    for (int i = 0; i < 4; i++) {
        char s1 = (char)fromSquare + offsets[i] + BOARD_WIDTH * offsets[(i <= 2) * 2];
        char s2 = (char)fromSquare + offsets[i] + BOARD_WIDTH * offsets[(i <= 2) * 2 + 1];

        board.AddPseudoLegalMove(ParseStdMove(board, index, s1));
        board.AddPseudoLegalMove(ParseStdMove(board, index, s2));
    }
}

void ParseRook(Board &board, char index) {
    int delta = index % 8;

    const int directions[] = {
            1, -1, 8, -8
    };
    const int bounds[] = {
            index + 7 - delta, index - delta, 64 - delta, 7 - delta
    };

    for (int i = 0; i < 4; i++) {
        int off = directions[i];

        for (char dI = index + off; dI != bounds[i]; dI += off) {
            if (board.Ally(dI))
                break;

            board.AddPseudoLegalMove(ParseStdMove(board, index, dI));

            if (!board.Empty(dI))
                break;
        }
    }
}

void ParseQueen(Board &board, char index) {
    ParseRook(board, index);
    ParseBishop(board, index);
}

void ParseKing(Board &board, char index) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;

            board.AddPseudoLegalMove(ParseStdMove(board, index, index + i + j * BOARD_WIDTH));
        }
    }
}
