#include <iostream>
#include <chrono>
#include "chess/Board.h"
#include "chess/BoardMove.h"

using namespace std;

void TestEngine(Board board);
void TestEngine2(Board board);
void PrintLegalMoves(Board board);
void MoveTest(Board& board, int depth, int &counter);

int main() {
    srand(time(nullptr));

    auto startTime = chrono::high_resolution_clock::now();

//    int runs = 1000;
//
//    auto start = std::chrono::high_resolution_clock::now();
//
//    for (int i = 0; i < runs; i++)
//        Board board("1R6/5pkp/p2Qp1p1/3p2q1/3P4/2P2P2/r4PPP/6K1 w - - 5 31", false);
//
//    auto end = std::chrono::high_resolution_clock::now();
//
//    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::nanoseconds>((end - start) / runs).count() << "ns" << std::endl;

    Board board;

    for (const Move& move : *board.GetPseudoLegalMoves()) {
        cout << (string) move.fromSquare << (string) move.toSquare << endl;
    }

//    int count = 0;
//    MoveTest(board, 1, count);
//    cout << "Move Count: " << count << endl;

    auto endTime = chrono::high_resolution_clock::now();

    cout << "Time taken: " << chrono::duration_cast<chrono::microseconds>(endTime - startTime).count() << " microseconds" << endl;

    board.PrintBoard();

    return 0;
}

void MoveTest(Board &board, int depth, int& counter) {
    counter++;

    if (depth == 0)
        return;

    for (const Move& move : *board.GetPseudoLegalMoves()) {
        board.PushMove(move);
        MoveTest(board, depth - 1, counter);
        board.UndoMove();
    }
}

void PrintLegalMoves(Board board) {
//    auto legalMoves = board.GetLegalMoves();

//    for (const auto& move : legalMoves) {
//        cout << move << endl;
//    }
}

//void TestEngine(Board board) {
//    Engine engine(6, 6000, board);
//
//    auto start = std::chrono::high_resolution_clock::now();
//
//    auto list = engine.GetBestMove();
//
//    auto end = std::chrono::high_resolution_clock::now();
//
//    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
//
//    for(const auto& move : list) {
//        std::cout << move << std::endl;
//    }
//}

//void TestEngine2(Board board) {
//    Engine engine(6, 6000, board);
//
//    float max = 1000;//(board.GetTurn()) ? -99 : 99;
//
//    board.PushMove("c3c4");
//
//    float eval;
//
//    eval = (board.GetTurn()) ?
//           engine.SearchEval(4, -9999, max, true)
//         :   engine.SearchEval(4,  max, 9999, false);
//
//    board.UndoMove();
//
//    cout << eval << endl;
//}