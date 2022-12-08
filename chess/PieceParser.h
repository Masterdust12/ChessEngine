//
// Created by zbv11 on 07-Dec-22.
//

#ifndef CHESSENGINE_PIECEPARSER_H
#define CHESSENGINE_PIECEPARSER_H

#include <list>
#include "BoardMove.h"
#include "Board.h"

void ParsePawn(Board& board, char index);
void ParseBishop(Board& board, char index);
void ParseKnight(Board& board, char index);
void ParseRook(Board& board, char index);
void ParseQueen(Board& board, char index);
void ParseKing(Board& board, char index);

#endif //CHESSENGINE_PIECEPARSER_H
