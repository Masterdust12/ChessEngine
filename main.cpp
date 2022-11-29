#include <iostream>
#include <chrono>
#include "chess/Board.h"
#include "Engine.h"

using namespace std;

void TestEngine(Board board);
void PrintLegalMoves(Board board);

int main() {
    srand(time(nullptr));

    Board board("r5k1/p1n1pr1p/6P1/2pPp1P1/4P2Q/1Pp5/1qP5/2RK2NR b - - 0 24", false);

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
//    board.PrintBoard();

    //board.PrintAttackedSquares();

    //PrintLegalMoves(board);
    TestEngine(board);
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