//
// Created by zbv11 on 05-Oct-22.
//

#ifndef CHESSENGINE_ENGINE_H
#define CHESSENGINE_ENGINE_H

#include "chess/Board.h"

class Engine {
public:
    explicit Engine(int depth, float searchTime, Board &board);

    std::list<std::string> GetBestMove();

    void SetSearchDepth(int depth);
    void SetSearchTime(float time);

    void AddPriorityMoves(std::list<std::string> &list);
    std::list<std::string> CreateOrderedMoves(std::list<std::string> priorities = {});

    float SearchEval(int depth, float alpha, float beta, bool maximizingPlayer);

    std::list<std::string> SearchMove(int depth, std::list<std::string> check_moves, float &eval);

    float MaterialDifference();

private:
    int searchDepth, nodesChecked;
    float searchTime;
    Board &board;

    float EvaluatePosition(int depth);
};


#endif //CHESSENGINE_ENGINE_H
