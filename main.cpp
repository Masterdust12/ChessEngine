#include <iostream>
#include "Board.h"

int main() {
    srand(time(nullptr));

    Board board;

    auto moves = board.GetLegalMoves();

//    for (auto move : moves) {
//        std::cout << move << std::endl;
//    }

    while (!board.IsGameEnd()) {
        std::list<std::string> moves = board.GetLegalMoves();

        // choose random move
        int move = rand() % moves.size();
        auto it = moves.begin();
        std::advance(it, move);

        board.PushMove(*it);
    }

    board.PrintMoveStack();
    board.PrintBoard();
}
