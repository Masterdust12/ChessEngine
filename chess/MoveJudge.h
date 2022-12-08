//
// Created by zbv11 on 07-Dec-22.
//

#ifndef CHESSENGINE_MOVEJUDGE_H
#define CHESSENGINE_MOVEJUDGE_H

#include <list>
#include "BoardMove.h"
#include "Board.h"

void FindPseudoLegalMoves(std::list<Move>& moves, Board& board);

#endif //CHESSENGINE_MOVEJUDGE_H
