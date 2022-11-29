#include <iostream>
#include <chrono>
#include "chess/Board.h"
#include "Engine.h"

using namespace std;

void TestEngine(Board board);
void TestEngine2(Board board);
void PrintLegalMoves(Board board);

int main() {
    srand(time(nullptr));

    Board board("1R6/5pkp/p2Qp1p1/3p2q1/3P4/2P2P2/r4PPP/6K1 w - - 5 31", false);

//    Engine testEngine(1, 100, board);
//
//    list<string> moves;
//
//    testEngine.AddPriorityMoves(moves);
//
//    for (const auto& move : moves) {
//        cout << move << endl;
//    }
//
    //PrintLegalMoves(board);
    board.PrintBoard();

    //board.PrintAttackedSquares();

    //PrintLegalMoves(board);
    //TestEngine(board);
    TestEngine2(board);
}

void PrintLegalMoves(Board board) {
    auto legalMoves = board.GetLegalMoves();

    for (const auto& move : legalMoves) {
        cout << move << endl;
    }
}

void TestEngine(Board board) {
    Engine engine(6, 6000, board);

    auto start = std::chrono::high_resolution_clock::now();

    auto list = engine.GetBestMove();

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    for(const auto& move : list) {
        std::cout << move << std::endl;
    }
}

void TestEngine2(Board board) {
    Engine engine(6, 6000, board);

    float max = 1000;//(board.GetTurn()) ? -99 : 99;

    board.PushMove("c3c4");

    float eval;

    eval = (board.GetTurn()) ?
           engine.SearchEval(4, -9999, max, true)
         :   engine.SearchEval(4,  max, 9999, false);

    board.UndoMove();

    cout << eval << endl;
}