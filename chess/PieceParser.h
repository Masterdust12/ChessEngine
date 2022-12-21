//
// Created by zbv11 on 07-Dec-22.
//

#ifndef CHESSENGINE_PIECEPARSER_H
#define CHESSENGINE_PIECEPARSER_H

#include <list>
#include <cstdint>
#include "Square.h"

class Board;

void ParsePawn(Board& board, const Square &index);
void ParseBishop(Board& board, const Square &index);
void ParseKnight(Board& board, const Square &index);
void ParseRook(Board& board, const Square &index);
void ParseQueen(Board& board, const Square &index);
void ParseKing(Board& board, const Square &index);

#endif //CHESSENGINE_PIECEPARSER_H
