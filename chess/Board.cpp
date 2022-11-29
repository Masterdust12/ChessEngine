//
// Created by zbv11 on 29-Sep-22.
//

#include "Board.h"
#include "../mmath.h"

#include <iostream>
#include <utility>
#include <string>

const char empty_square = '.';
const int castle_legal = 999999;

Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

Board::Board(std::string FEN, bool fauxStart) {
    turn = true;
    board = std::array<char, 64>();
    ParseFEN(std::move(FEN));

    if (!fauxStart)
        Inspect();
}

void Board::PushMove(const std::string& move) {
    std::string exact_move;

    for (std::string s : legalMoves) {
        if (s.substr(0, 4) == move.substr(0, 4)) {
            exact_move = s;
            break;
        }
    }

    if (exact_move.empty()) {
        PrintBoard();

        std::cout << "Could not find move: " << move << std::endl;
        PrintMoveStack();
        return;
    }

    SoftPushMove(exact_move);

    Inspect();
}

void Board::SoftPushMove(const std::string& move) {
    int fromIndex, toIndex;
    char promotion;

    ParseMove(move, fromIndex, toIndex, promotion);

    if (move == "e1g1" && castling_rights[0]) {
        std::swap(board[fromIndex], board[toIndex]);
        std::swap(board[Index("h1")], board[toIndex - 1]);
    }
    else if (move == "e1c1" && castling_rights[1]) {
        std::swap(board[fromIndex], board[toIndex]);
        std::swap(board[Index("a1")], board[toIndex + 1]);
    }
    else if (move == "e8g8" && castling_rights[2]) {
        std::swap(board[fromIndex], board[toIndex]);
        std::swap(board[Index("h8")], board[toIndex - 1]);
    }
    else if (move == "e8c8" && castling_rights[3]) {
        std::swap(board[fromIndex], board[toIndex]);
        std::swap(board[Index("a8")], board[toIndex + 1]);
    } else goto normal_move;

    moves.push(move);

    turn = !turn;

    for (int & castling_right : castling_rights) {
        castling_right = std::min(castling_right, (int) moves.size());
    }

    return;

    normal_move:
    std::swap(board[fromIndex], board[toIndex]);

    board[fromIndex] = empty_square;

    if (promotion != empty_square) {
        board[toIndex] = promotion;
    }

    int minus = move.find('-');

    if (minus != std::string::npos) {
        std::string square = move.substr(minus + 1, 2);

        board[Index(square)] = empty_square;
    }

    if (fromIndex == Index("a1") && board[toIndex] == 'R')
        castling_rights[0] = std::min(castling_rights[0], (int) moves.size());
    else if (fromIndex == Index("h1") && board[toIndex] == 'R')
        castling_rights[1] = std::min(castling_rights[1], (int) moves.size());
    else if (fromIndex == Index("a8") && board[toIndex] == 'r')
        castling_rights[2] = std::min(castling_rights[2], (int) moves.size());
    else if (fromIndex == Index("h8") && board[toIndex] == 'r')
        castling_rights[3] = std::min(castling_rights[3], (int) moves.size());
    else if (fromIndex == Index("e1") && board[toIndex] == 'K') {
        castling_rights[0] = std::min(castling_rights[0], (int) moves.size());
        castling_rights[1] = std::min(castling_rights[1], (int) moves.size());
    } else if (fromIndex == Index("e8") && board[toIndex] == 'k') {
        castling_rights[2] = std::min(castling_rights[2], (int) moves.size());
        castling_rights[3] = std::min(castling_rights[3], (int) moves.size());
    }

    moves.push(move);

    turn = !turn;
}

std::string Board::UndoMove() {
    std::string move = SoftUndoMove();

    Inspect();

    return move;
}

std::string Board::SoftUndoMove() {
    if (moves.empty()) {
        std::cout << "No moves to undo" << std::endl;
        return "";
    }

    std::string move = moves.top();
    moves.pop();

    int fromIndex, toIndex;
    char promotion;

    char capturedPiece = ParseMove(move, fromIndex, toIndex, promotion);

    if (move == "e1g1" && board[fromIndex] == 'K') {
        std::swap(board[fromIndex], board[toIndex]);
        std::swap(board[Index("a1")], board[toIndex + 1]);
    }
    else if (move == "e1c1" && board[fromIndex] == 'K') {
        std::swap(board[fromIndex], board[toIndex]);
        std::swap(board[Index("h1")], board[toIndex - 1]);
    }
    else if (move == "e8g8" && board[fromIndex] == 'k') {
        std::swap(board[fromIndex], board[toIndex]);
        std::swap(board[Index("a8")], board[toIndex + 1]);
    }
    else if (move == "e8c8" && board[fromIndex] == 'k') {
        std::swap(board[fromIndex], board[toIndex]);
        std::swap(board[Index("h8")], board[toIndex - 1]);
    } else goto normal_move;

    for (int & castling_right : castling_rights) {
        if (castling_right == moves.size())
            castling_right = castle_legal;
    }

    turn = !turn;
    return move;

    normal_move:
    std::swap(board[toIndex], board[fromIndex]);

    board[toIndex] = capturedPiece;

    turn = !turn;

    if (promotion != empty_square) {
        board[fromIndex] = turn ? 'P' : 'p';
    }

    int minus = move.find('-');

    if (minus != std::string::npos) {
        std::string square = move.substr(minus + 1, 2);

        board[Index(square)] = turn ? 'p' : 'P';
    }

    if (fromIndex == Index("e1") && board[fromIndex] == 'K' && castling_rights[0] == moves.size())
        castling_rights[0] = castle_legal;
    else if (fromIndex == Index("e1") && board[fromIndex] == 'K' && castling_rights[1] == moves.size())
        castling_rights[1] = castle_legal;
    else if (fromIndex == Index("e8") && board[fromIndex] == 'k' && castling_rights[2] == moves.size())
        castling_rights[2] = castle_legal;
    else if (fromIndex == Index("e8") && board[fromIndex] == 'k' && castling_rights[3] == moves.size())
        castling_rights[3] = castle_legal;

    return move;
}

void Board::ParseFEN(std::string FEN) {
    board.fill('.');

    int offset = 0, i = 0;

    for (i = 0; i < FEN.length(); i++) {
        char c = FEN[i];

        if (isalpha(c)) {
            board[i + offset] = c;
        } else if (isdigit(c)) {
            offset += (c - '0') - 1;
        } else if (c == '/') {
            offset--;
        }

        if (c == ' ') {
            turn = FEN[i + 1] == 'w';
            i += 3;
            break;
        }
    }

    while (FEN[i] != ' ') {
        switch(FEN[i]) {
            case 'K':
                castling_rights[0] = castle_legal;
                break;
            case 'Q':
                castling_rights[1] = castle_legal;
                break;
            case 'k':
                castling_rights[2] = castle_legal;
                break;
            case 'q':
                castling_rights[3] = castle_legal;
                break;
        }
        i++;
    }

    i++;
}

char Board::ParseMove(std::string move, int &fromIndex, int &toIndex, char& promotion) {
    std::string fromSquare = move.substr(0, 2);
    std::string toSquare = move.substr(2, 2);
    char capture = empty_square;
    promotion = empty_square;

    fromIndex = Index(fromSquare);
    toIndex = Index(toSquare);

    for (int i = 0; i < move.length() - 1; i++) {
        if (move[i] == 'x')
            capture = move[i + 1];
        if (move[i] == '=')
            promotion = move[i + 1];
    }

    return capture;
}

void Board::PrintBoard() {
    std::cout << "------------------------" << std::endl;
    for (int i = 0; i < 64; i++) {
        std::cout << board[i] << " ";
        if (i % 8 == 7) {
            std::cout << std::endl;
        }
    }
    std::cout << "------------------------" << std::endl;
}

void Board::PrintAttackedSquares() {
std::cout << "------------------------" << std::endl;
    for (int i = 0; i < 64; i++) {
        std::cout << attacked_squares[i] << " ";
        if (i % 8 == 7) {
            std::cout << std::endl;
        }
    }
    std::cout << "------------------------" << std::endl;
}

void Board::Inspect() {
    pseudoLegalCaptures.clear();
    pseudoLegalMoves.clear();
    legalMoves.clear();
    legalCaptures.clear();
    attacked_squares.fill(0);

    GenAttackedSquares(attacked_squares);
    GenPseudoLegalMoves(pseudoLegalMoves);
    GenCaptures(pseudoLegalMoves, pseudoLegalCaptures);
    PruneNonKosherMoves(pseudoLegalMoves, legalMoves);
    GenCaptures(legalMoves, legalCaptures);
}

void Board::GenAttackedSquares(std::array<int, 64> &squares) {
    turn = !turn;
    std::list<std::string> temp_moves;
    GenPseudoLegalMoves(temp_moves, true);
    turn = !turn;

    for (const auto& pseudo_move : temp_moves) {
        int fromIndex, toIndex;
        char g;

        ParseMove(pseudo_move, fromIndex, toIndex, g);

        if (tolower(board[fromIndex]) == 'p' && abs(fromIndex - toIndex) % 8 == 0)
            continue;

        squares[toIndex]++;
    }
}

void Board::GenPseudoLegalMoves(std::list<std::string> &list, bool ignore_pawn_rules) {
    for (int i = 0; i < board.size(); i++) {
        int piece = tolower(board[i]);

        if (isupper(board[i]) != turn)
            continue;

        switch (piece) {
            case 'p':
                ParsePawn(list, i, ignore_pawn_rules);
                break;
            case 'r':
                ParseRook(list, i);
                break;
            case 'n':
                ParseKnight(list, i);
                break;
            case 'b':
                ParseBishop(list, i);
                break;
            case 'q':
                ParseQueen(list, i);
                break;
            case 'k':
                ParseKing(list, i);
                break;
            default:
                break;
        }
    }
}

void Board::PruneNonKosherMoves(std::list<std::string> &non_pruned, std::list<std::string> &pruned) {
    for (const std::string& move : non_pruned) {
        SoftPushMove(move);

        if (!IsSoftCheck())
            pruned.push_back(move);

        SoftUndoMove();
    }

    int whiteKing = 0, blackKing = 0;

    for (int i = 0; i < board.size(); i++) {
        if (board[i] == 'K')
            whiteKing = i;
        else if (board[i] == 'k')
            blackKing = i;
    }

    // Add Castling
    if (turn && castling_rights[0] > moves.size() && SquareSafeAndEmpty("c1") && SquareSafeAndEmpty("d1") && !IsCheck()) {
        AddMove(pruned, whiteKing, Index("c1"));
    }
    if (turn && castling_rights[1] > moves.size() && SquareSafeAndEmpty("f1") && SquareSafeAndEmpty("g1") && !IsCheck()) {
        AddMove(pruned, whiteKing, Index("g1"));
    }
    if (!turn && castling_rights[2] > moves.size() && SquareSafeAndEmpty("c8") && SquareSafeAndEmpty("d8") && !IsCheck()) {
        AddMove(pruned, blackKing, Index("c8"));
    }
    if (!turn && castling_rights[3] > moves.size() && SquareSafeAndEmpty("f8") && SquareSafeAndEmpty("g8") && !IsCheck()) {
        AddMove(pruned, blackKing, Index("g8"));
    }
}

void Board::GenCaptures(std::list<std::string> &move_list, std::list<std::string> &capture_list) {
    for (const std::string& move : move_list) {
        int fromIndex, toIndex;
        char g;

        ParseMove(move, fromIndex, toIndex, g);

        if (board[toIndex] != empty_square) {
            capture_list.push_back(move);
        }
    }
}

void Board::ParseBishop(std::list<std::string> &move_list, int index) {
    int rank = Rank(index);
    int file = File(index);

    // Up Right
    for (int i = 1; i <= 7; i++) {
        int ri = rank + i;
        int fi = file + i;

        if (ri > 7 || fi > 7)
            break;

        AddMove(move_list, index, Index(ri, fi));

        if (!Empty(ri, fi))
            break;
    }

    // Up Left
    for (int i = 1; i <= 7; i++) {
        int ri = rank + i;
        int fi = file - i;

        if (ri > 7 || fi < 0)
            break;

        AddMove(move_list, index, Index(ri, fi));

        if (!Empty(ri, fi))
            break;
    }

    // Down Right
    for (int i = 1; i <= 7; i++) {
        int ri = rank - i;
        int fi = file + i;

        if (ri < 0 || fi > 7)
            break;

        AddMove(move_list, index, Index(ri, fi));

        if (!Empty(ri, fi))
            break;
    }

    // Down Left
    for (int i = 1; i <= 7; i++) {
        int ri = rank - i;
        int fi = file - i;

        if (ri < 0 || fi < 0)
            break;

        AddMove(move_list, index, Index(ri, fi));

        if (!Empty(ri, fi))
            break;
    }
}

void Board::ParseKing(std::list<std::string> &move_list, int index) {
    int rank = Rank(index);
    int file = File(index);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            int xx = file + x;
            int yy = rank + y;

            if (xx < 0 || xx > 7 || yy < 0 || yy > 7 || (x == y && x == 0))
                continue;

            AddMove(move_list, index, Index(yy, xx));
        }
    }

    bool safe1 = SquareSafeAndEmpty("f1");
    bool safe2 = SquareSafeAndEmpty("g1");
}

void Board::ParseKnight(std::list<std::string> &move_list, int index) {
    int rank = Rank(index);
    int file = File(index);

    // I have no motivation to write this in a loop
    AddMove(move_list, index, Index(rank + 2, file + 1));
    AddMove(move_list, index, Index(rank + 2, file - 1));
    AddMove(move_list, index, Index(rank - 2, file + 1));
    AddMove(move_list, index, Index(rank - 2, file - 1));
    AddMove(move_list, index, Index(rank + 1, file + 2));
    AddMove(move_list, index, Index(rank + 1, file - 2));
    AddMove(move_list, index, Index(rank - 1, file + 2));
    AddMove(move_list, index, Index(rank - 1, file - 2));
}

void Board::ParsePawn(std::list<std::string> &move_list, int index, bool ignore_pawn_rules) {
    bool isWhite = isupper(board[index]);
    int rank = Rank(index);
    int file = File(index);

    if (isWhite) {
        std::list<int> squares;

        if (Enemy(rank + 1, file - 1) || ignore_pawn_rules)
            squares.push_back(index - 9);
        if (Enemy(rank + 1, file + 1) || ignore_pawn_rules)
            squares.push_back(index - 7);
        if (Empty(rank + 1, file))
            squares.push_back(index - 8);
        if (rank == 1 && Empty(rank - 2, file))
            squares.push_back(index - 16);

        for (auto square : squares) {
            if (Rank(square) == 7) {
                AddMove(move_list, index, square, 'q');
                AddMove(move_list, index, square, 'r');
                AddMove(move_list, index, square, 'b');
                AddMove(move_list, index, square, 'n');
            } else {
                AddMove(move_list, index, square);
            }
        }

        if (rank == 4 && !moves.empty()) {
            std::string last_move = moves.top();
            int fromIndex, toIndex;
            char g;

            ParseMove(last_move, fromIndex, toIndex, g);

            if (file > 0 && board[index - 1] == 'p' && toIndex == index - 1 && fromIndex == index - 17)
                AddMove(move_list, index, index - 9, 0, true);
            if (file < 7 && board[index + 1] == 'p' && toIndex == index + 1 && fromIndex == index - 15)
                AddMove(move_list, index, index - 7, 0, true);
        }
    } else {
        std::list<int> squares;

        if (Enemy(rank - 1, file - 1) || ignore_pawn_rules)
            squares.push_back(index + 7);
        if (Enemy(rank - 1, file + 1) || ignore_pawn_rules)
            squares.push_back(index + 9);
        if (Empty(rank - 1, file))
            squares.push_back(index + 8);
        if (rank == 1 && Empty(rank - 2, file))
            squares.push_back(index + 16);

        for (auto square : squares) {
            if (Rank(square) == 7) {
                AddMove(move_list, index, square, 'q');
                AddMove(move_list, index, square, 'r');
                AddMove(move_list, index, square, 'b');
                AddMove(move_list, index, square, 'n');
            } else {
                AddMove(move_list, index, square);
            }
        }

        if (rank == 3 && !moves.empty()) {
            std::string last_move = moves.top();
            int fromIndex, toIndex;
            char g;

            ParseMove(last_move, fromIndex, toIndex, g);

            if (file > 0 && board[index - 1] == 'P' && toIndex == index - 1 && fromIndex == index + 15)
                AddMove(move_list, index, index + 7, 0, true);
            if (file < 7 && board[index + 1] == 'P' && toIndex == index + 1 && fromIndex == index + 17)
                AddMove(move_list, index, index + 9, 0, true);
        }
    }
}

void Board::ParseQueen(std::list<std::string> &move_list, int index) {
    ParseBishop(move_list, index);
    ParseRook(move_list, index);
}

void Board::ParseRook(std::list<std::string> &move_list, int index) {
    int rank = Rank(index);
    int file = File(index);

    // Up
    for (int i = 1; i <= 7; i++) {
        int ri = rank + i;
        int fi = file;

        if (ri > 7)
            break;

        if (Ally(ri, fi))
            break;

        AddMove(move_list, index, Index(ri, fi));

        if (!Empty(ri, fi))
            break;
    }

    // Down
    for (int i = 1; i <= 7; i++) {
        int ri = rank - i;
        int fi = file;

        if (ri < 0)
            break;

        if (Ally(ri, fi))
            break;

        AddMove(move_list, index, Index(ri, fi));

        if (!Empty(ri, fi))
            break;
    }

    // Right
    for (int i = 1; i <= 7; i++) {
        int ri = rank;
        int fi = file + i;

        if (fi > 7)
            break;

        if (Ally(ri, fi))
            break;

        AddMove(move_list, index, Index(ri, fi));

        if (!Empty(ri, fi))
            break;
    }

    // Left
    for (int i = 1; i <= 7; i++) {
        int ri = rank;
        int fi = file - i;

        if (fi < 0)
            break;

        if (Ally(ri, fi))
            break;

        AddMove(move_list, index, Index(ri, fi));

        if (!Empty(ri, fi))
            break;
    }
}

int Board::Rank(int index) {
    return 7 - index / 8;
}

int Board::File(int index) {
    return index % 8;
}

std::string Board::Square(int index) {
    return std::string(1, 'a' + File(index)) + std::string(1, Rank(index) + '1');
}

std::string Board::Square(int rank, int file) {
    return Square(Index(rank, file));
}

int Board::Index(int rank, int file) {
    if (rank < 0 || rank > 7 || file < 0 || file > 7)
        return -1;

    return (7 - rank) * 8 + file;
}

bool Board::NotAlly(int index) {
    return Empty(index) || Enemy(index);
}

bool Board::Ally(int rank, int file) {
    return Ally(Index(rank, file));
}

bool Board::Ally(int index) {
    return !Empty(index) && isupper(board[index]) == turn;
}

bool Board::Enemy(int index) {
    return !Empty(index) && isupper(board[index]) != turn;
}

bool Board::Enemy(int rank, int file) {
    if (rank < 0 || rank > 7 || file < 0 || file > 7)
        return false;

    return Enemy(Index(rank, file));
}

bool Board::Empty(int index) {
    return index != -1 && board[index] == empty_square;
}

bool Board::Empty(int rank, int file) {
    if (rank < 0 || rank > 7 || file < 0 || file > 7)
        return false;

    return board[Index(rank, file)] == empty_square;
}

bool Board::IsCheck() {
    return attacked_squares[FindKing()];
}

std::string Board::GenMove(int fromSquare, int toSquare, char promotion, bool en_passant) {
    std::string baseMove = Square(fromSquare) + Square(toSquare);
    std::string capture = (board[toSquare] == empty_square) ? "" : "x" + std::string(1, board[toSquare]);
    std::string prom = (promotion == 0) ? "" : ("=" + std::string(1, promotion));

    int diff = fromSquare - toSquare;

    std::string enpass = en_passant ? "-" + Square(fromSquare + mmath::sign(diff) * 8 - diff) : "";

    return baseMove + capture + prom + enpass;
}

void Board::AddMove(std::list<std::string>& move_list, int fromSquare, int toSquare, char promotion, bool en_passant) {
    if (toSquare == -1 || fromSquare == -1)
        return;

    std::string move = GenMove(fromSquare, toSquare, promotion, en_passant);

    if (fromSquare < 0 || fromSquare > 63 || toSquare < 0 || toSquare > 63)
        return;

    if (NotAlly(toSquare))
        move_list.push_back(move);
}

std::list<std::string> Board::GetLegalMoves() {
    return legalMoves;
}

std::list<std::string> Board::GetLegalCaptures() {
    return legalCaptures;
}

std::list<std::string> Board::GetPseudoLegalMoves() {
    return pseudoLegalMoves;
}

std::list<std::string> Board::GetPseudoLegalCaptures() {
    return pseudoLegalCaptures;
}

char Board::GetPieceAt(int rank, int file) const {
    return board[Index(rank, file)];
}

int Board::Index(std::string square){
    return Index(square[1] - '1', tolower(square[0] - 'a'));
}

void Board::PrintMoveStack() {
    std::stack<std::string> temp;

    while (!moves.empty()) {
        temp.push(moves.top());
        moves.pop();
    }

    while (!temp.empty()) {
        std::cout << temp.top() << std::endl;
        moves.push(temp.top());
        temp.pop();
    }
}

bool Board::RaycastReflect(int from, int aim, int ignores, int *squareHit) {
    int rankDiff = Rank(aim) - Rank(from);
    int fileDiff = File(aim) - File(from);

    // if the direction of the ray is not diagonal or straight, return false
    if (abs(rankDiff) != abs(fileDiff) && rankDiff != 0 && fileDiff != 0)
        return false;

    bool diagonal = abs(rankDiff) == abs(fileDiff);

    rankDiff = mmath::sign(rankDiff);
    fileDiff = mmath::sign(fileDiff);

    int rank = Rank(from) + rankDiff;
    int file = File(from) + fileDiff;

    while (rank >= 0 && rank <= 7 && file >= 0 && file <= 7) {
        if (!Empty(rank, file))
            if (!Ally(rank, file) && ignores > 0) {
                ignores--;
            } else if (Ally(rank, file)) {
                if (tolower(GetPieceAt(rank, file)) == 'r' && !diagonal
                    || tolower(GetPieceAt(rank, file)) == 'b' && diagonal
                    || tolower(GetPieceAt(rank, file)) == 'q') {

                    if (squareHit != nullptr)
                        *squareHit = Index(rank, file);

                    return true;
                } else
                    return false;
            } else if (Enemy(rank, file))
                return false;

        rank += rankDiff;
        file += fileDiff;
    }

    return false;
}

bool Board::IsSoftCheck() {
    if (moves.empty())
        throw std::runtime_error("IsSoftCheck() called without a move on the stack");

    int from, to;
    char prom;

    ParseMove(moves.top(), from, to, prom);

    // If the player moved the king to a square that is attacked, that will always be a check
    if (board[to] == (!turn ? 'K' : 'k'))
        return attacked_squares[to] || RaycastReflect(to, from) || RaycastReflect(from, to, 1);

    int kingIndex = FindEnemyKing();

    switch (attacked_squares[kingIndex]) {
        case 2:
            // Case 1: The king is in double check, so the king needs to be moved
            return true;
        case 1:
            // Case 2: The square was under attack before once, check to see if the move was a block.
            return RaycastReflect(kingIndex, to) || !RaycastReflect(kingIndex, to, 1);
        case 0:
            // Case 3: The square was not attacked before, check to make sure the piece that was moved was not pinned.
            return RaycastReflect(kingIndex, from);
    }

    // I don't think this should ever happen, but just in case
    // I hate that GitHub CoPilot generated that above comment, wtf
    return true;
}

bool Board::SquareSafeAndEmpty(const std::string& square) {
    return !attacked_squares[Index(square)] && board[Index(square)] == empty_square;
}

bool Board::IsStalemate() {
    return !IsCheck() && legalMoves.empty();
}

bool Board::IsCheckmate() {
    return IsCheck() && legalMoves.empty();
}

bool Board::IsGameEnd() {
    return IsCheckmate() || IsStalemate() || moves.size() >= 100;
}

int Board::FindKing() {
    for (int i = 0; i < 64; i++) {
        if (board[i] == 'k' && !turn || board[i] == 'K' && turn)
            return i;
    }

    return -1;
}

int Board::FindEnemyKing() {
    for (int i = 0; i < 64; i++) {
        if (board[i] == 'k' && turn || board[i] == 'K' && !turn)
            return i;
    }

    return -1;
}

bool Board::IsAttacked(int index) {
    return attacked_squares[index];
}

char Board::GetPieceAt(int x) const {
    return board[x];
}

char Board::GetPieceAt(std::string square) const {
    return board[Index(std::move(square))];
}

bool Board::GetTurn() {
    return turn;
}

bool Board::SoftCheck() {
    std::array<int, 64> temp{};
    GenAttackedSquares(temp);

    return temp[FindKing()] > 0;
}
