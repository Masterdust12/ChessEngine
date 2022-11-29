//
// Created by zbv11 on 05-Oct-22.
//

#include <iostream>
#include "Engine.h"
#include "mmath.h"

Engine::Engine(int depth, float searchTime, Board &board) : board(board) {
    this->board = board;
    this->searchDepth = depth;
    this->searchTime = searchTime;
}

std::list<std::string> Engine::GetBestMove() {
    auto legalMoves = board.GetLegalMoves();

    float start_time = clock();

    std::list<std::string> best_moves;

    nodesChecked = 0;

    for(int depth = 1; depth < searchDepth; depth++) {
        float eval = 0;
        auto moves = SearchMove(depth, best_moves, eval);
        std::cout << "Searched Depth: " << depth << " Best Move: " << moves.front() << " Eval: " << eval << std::endl;

        best_moves.clear();
        best_moves = std::move(moves);

        // If the search time has been exceeded, break out of the loop
        if((clock() - start_time) / CLOCKS_PER_SEC > searchTime || std::abs(eval) > 99 || legalMoves.size() == 1) {
            break;
        }
    }

    std::cout << "Nodes Checked: " << nodesChecked << std::endl;

    return best_moves;
}

std::list<std::string> Engine::SearchMove(int depth, std::list<std::string> check_moves, float &final_eval) {
    std::list<std::string> best_moves;

    // copy check list to local variable
    std::list<std::string> moves = CreateOrderedMoves(std::move(check_moves));

    float max = board.GetTurn() ? -99 : 99;

    for (const auto& move : moves) {

        if (move == "c3c4")
            int x = 0;

        board.PushMove(move);

        float eval;

        eval = (board.GetTurn()) ?
                SearchEval(depth - 1, -99, max, true)
            :   SearchEval(depth - 1,  max, 99, false);

        board.UndoMove();

        if (mmath::SidedCompare(eval, max, board.GetTurn())) {
            max = eval;
            best_moves.clear();
            best_moves.push_back(move);
        } else if (eval == max) {
            best_moves.push_back(move);
        }
    }

    final_eval = max;
    return best_moves;
}

float Engine::SearchEval(int depth, float alpha, float beta, bool maximizingWhite) {
    nodesChecked++;

    std::list<std::string> moves = CreateOrderedMoves();

    if (depth == 0 || board.IsGameEnd())
        return EvaluatePosition(depth);

    if (maximizingWhite) {
       float value = -INT16_MAX;
       for (const auto& move : moves) {
           board.PushMove(move);
           value = std::max(value, SearchEval(depth - 1, alpha, beta, false));
           board.UndoMove();

           if (value >= beta)
               break;

           alpha = std::max(alpha, value);
       }
       return value;
    } else {
        float value = INT16_MAX;
        for (const auto& move : moves) {
            board.PushMove(move);
            value = std::min(value, SearchEval(depth - 1, alpha, beta, true));
            board.UndoMove();

            if (value <= alpha)
                break;

            beta = std::min(beta, value);
        }
        return value;
    }
}

void Engine::AddPriorityMoves(std::list<std::string> &priorities) {
    auto moves = board.GetLegalMoves();

    for (const auto& move : moves) {
        for (const auto& prio : priorities) {
            if (prio == move)
                goto next_move;
        }

        int from, to;
        char prom;

        Board::ParseMove(move, from, to, prom);

        if (board.Enemy(to)) {
            priorities.push_back(move);
            continue;
        }

        std::swap(board.board[from], board.board[to]);

        board.turn = !board.turn;

        if (board.RaycastReflect(board.FindKing(), to)) {
            priorities.push_back(move);
        }

        board.turn = !board.turn;

        std::swap(board.board[from], board.board[to]);

        next_move:
        continue;
    }
}

std::list<std::string> Engine::CreateOrderedMoves(std::list<std::string> priorities) {
    std::list<std::string> ordered_moves = std::move(priorities);

    AddPriorityMoves(ordered_moves);

    for (const auto& move : board.GetLegalMoves()) {
        for (const auto& prio : ordered_moves) {
            if (prio == move)
                goto next_move;
        }

        ordered_moves.push_back(move);

        next_move:
        continue;
    }

    return ordered_moves;
}

float Engine::EvaluatePosition(int depth) {
    if (board.IsCheckmate()) {
        return 1000 * (board.GetTurn() * -2 + 1) * (depth + 1);
    } else if (board.IsStalemate()) {
        return 0;
    }

    return MaterialDifference();
}

float Engine::MaterialDifference() {
    float runningTotal = 0;

    for (int i = 0; i < 64; i++) {
        float worth = 0;

        switch (tolower(board.GetPieceAt(i))) {
            case 'p':
                worth = 1;
                break;
            case 'n':
            case 'b':
                worth = 3;
                break;
            case 'r':
                worth = 5;
                break;
            case 'q':
                worth = 9;
                break;
            default:
                break;
        }

        worth *= isupper(board.GetPieceAt(i)) ? 1 : -1;

        runningTotal += worth;
    }

    return runningTotal;
}

void Engine::SetSearchDepth(int depth) {
    searchDepth = depth;
}

void Engine::SetSearchTime(float time) {
    searchTime = time;
}
