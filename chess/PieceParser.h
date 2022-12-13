//
// Created by zbv11 on 07-Dec-22.
//

#ifndef CHESSENGINE_PIECEPARSER_H
#define CHESSENGINE_PIECEPARSER_H

#include <list>
#include <cstdint>

class Board;

void ParsePawn(Board& board, int8_t index);
void ParseBishop(Board& board, int8_t index);
void ParseKnight(Board& board, int8_t index);
void ParseRook(Board& board, int8_t index);
void ParseQueen(Board& board, int8_t index);
void ParseKing(Board& board, int8_t index);

#endif //CHESSENGINE_PIECEPARSER_H
