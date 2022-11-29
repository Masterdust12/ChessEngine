#include <iostream>
#include <chrono>
#include "chess/Board.h"
#include "Engine.h"

using namespace std;

int main() {
    srand(time(nullptr));

//    Board moveTest("8/8/5b2/8/3R4/7k/1K6/8 w - - 0 1", false);
//
//    moveTest.SoftPushMove("d4d5");
//
//    moveTest.PrintBoard();
//
//    std::list<std::string> moves = moveTest.GetLegalMoves();
//
//    for (auto move : moves) {
//        std::cout << move << std::endl;
//    }

    Board board("r5k1/p1n1pr1p/6P1/2pPp1P1/4P2Q/1Pp5/1qP5/2RK2NR b - - 0 24", false);

//    board.SoftPushMove("c1b1");

//    board.PrintBoard();
//    board.PrintAttackedSquares();

//    cout << board.RaycastReflect(Board::Index("b1"), Board::Index("c1")) << endl;

//    for (auto move : board.GetLegalMoves()) {
//        std::cout << move << std::endl;
//    }

    Engine engine(100, 6000, board);

//    std::cout << engine.SearchEval(2, -999, 999, false) << std::endl;

//    float eval;
//    auto m = engine.SearchMove(3, { "c7f7" }, eval);
//
//    for (auto move : m) {
//        std::cout << move << std::endl;
//    }
//    std::cout << eval << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    auto list = engine.GetBestMove();

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    for(const auto& move : list) {
        std::cout << move << std::endl;
    }
}
