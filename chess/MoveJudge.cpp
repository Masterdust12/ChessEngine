//
// Created by zbv11 on 03-Dec-22.
//

#include "Board.h"
#include "PieceParser.h"

void FindPseudoLegalMoves(Board &board) {
    for (char i = 0; i < BOARD_SIZE; i++) {
        if (!board.Ally(i))
            continue;

        int piece = tolower(board.GetPieceAt(i));

        switch (piece) {
            case 'p':
                ParsePawn(board, i);
                break;
//            case 'r':
//                ParseRook(board, i);
//                break;
//            case 'n':
//                ParseKnight(board, i);
//                break;
//            case 'b':
//                ParseBishop(board, i);
//                break;
//            case 'q':
//                ParseQueen(board, i);
//                break;
//            case 'k':
//                ParseKing(board, i);
//                break;
            default:
                break;
        }
    }
}
