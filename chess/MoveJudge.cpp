//
// Created by zbv11 on 03-Dec-22.
//

#include "MoveJudge.h"
#include "PieceParser.h"
#include "Board.h"

void FindPseudoLegalMoves(std::list<Move>& moves, Board& board) {
    for (char i = 0; i < BOARD_SIZE; i++) {
        if (!board.Ally(i))
            continue;

        int piece = tolower(board.GetPieceAt(i));

        switch (piece) {
            case 'p':
                ParsePawn(moves, board, i);
                break;
            case 'r':
                ParseRook(moves, board, i);
                break;
            case 'n':
                ParseKnight(moves, board, i);
                break;
            case 'b':
                ParseBishop(moves, board, i);
                break;
            case 'q':
                ParseQueen(moves, board, i);
                break;
            case 'k':
                ParseKing(moves, board, i);
                break;
            default:
                break;
        }
    }
}
