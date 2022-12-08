//
// Created by zbv11 on 07-Dec-22.
//

#ifndef CHESSENGINE_ENGINE_H
#define CHESSENGINE_ENGINE_H

#include <list>
#include <string>

void InitEngine(int searchDepth, float maxSearchTime);

std::list<std::string> GetBestMove(Board board);
std::list<std::string> SearchMove(Board board, int depth, std::list<std::string> check_moves, float &final_eval);
float SearchEval(int depth, float alpha, float beta, bool maximizingWhite);
void AddPriorityMoves(Board board, std::list<Move> &priorities);
std::list<std::string> CreateOrderedMoves(std::list<std::string> priorities);
float EvaluatePosition(Board board, int depth);
float MaterialDifference(Board board);

#endif //CHESSENGINE_ENGINE_H
