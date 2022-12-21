//
// Created by zbv11 on 29-Sep-22.
//

#include "Board.h"
#include "MoveJudge.h"
#include <string>
#include <iostream>

Board::Board() : Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}

Board::Board(std::string FEN, bool fauxStart, bool debug) {
    turn = true;

    pseudoLegalMoves = new std::list<Move>();

    ParseFEN(std::move(FEN));

    if (!fauxStart)
        InspectBoard();
}

Board::~Board() {
    delete pseudoLegalMoves;
}

void Board::InspectBoard() {
    pastMoveLists.push(pseudoLegalMoves);
    pseudoLegalMoves = new std::list<Move>;

    FindPseudoLegalMoves(*this);
}

void Board::ParseFEN(std::string FEN) {
    board.fill('.');

    int offset = 0;
    size_t i;

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
                castling_rights[0] = CASTLE_LEGAL;
                break;
            case 'Q':
                castling_rights[1] = CASTLE_LEGAL;
                break;
            case 'k':
                castling_rights[2] = CASTLE_LEGAL;
                break;
            case 'q':
                castling_rights[3] = CASTLE_LEGAL;
                break;
        }
        i++;
    }
}


bool Board::WhitePiece(int8_t index) const {
    return isupper(GetPieceAt(index));
}

bool Board::PieceTurnRelation(int8_t index) const {
    return WhitePiece(index) == turn;
}

bool Board::Empty(int8_t index) const {
    return IsPieceAt(index, EMPTY_SQUARE);
}

bool Board::Ally(int8_t index) const {
    return !Empty(index) && PieceTurnRelation(index);
}

bool Board::Enemy(int8_t index) const {
    return !Empty(index) && !PieceTurnRelation(index);
}

bool Board::Empty(int8_t index, int8_t fileOffset, int8_t rankOffset) const {
    return Empty(index + fileOffset - rankOffset * BOARD_DIM);
}

bool Board::Ally(int8_t index, int8_t fileOffset, int8_t rankOffset) const {
    return Ally(index + fileOffset - rankOffset * BOARD_DIM);
}

bool Board::Enemy(int8_t index, int8_t fileOffset, int8_t rankOffset) const {
    return Enemy(index + fileOffset - rankOffset * BOARD_DIM);
}

char Board::GetPieceAt(char index) const {
    return board[index];
}

bool Board::IsPieceAt(int8_t index, char piece) const {
    return GetPieceAt(index) == piece;
}

void Board::SetPieceAt(int8_t index, char piece) {
    board[index] = piece;
}

int8_t Board::GetPieceAt(int8_t index, int8_t fileOffset, int8_t rankOffset) const {
    return GetPieceAt(index + fileOffset + rankOffset * BOARD_DIM);
}

bool Board::IsPieceAt(int8_t index, int8_t fileOffset, int8_t rankOffset, char piece) const {
    return IsPieceAt(index + fileOffset + rankOffset * BOARD_DIM, piece);
}

void Board::SetPieceAt(int8_t index, int8_t fileOffset, int8_t rankOffset, char piece) {
    SetPieceAt(index + fileOffset + rankOffset * BOARD_DIM, piece);
}

void Board::AddPseudoLegalMove(const Move &move) {
    if (move.Invalid() || Ally(move.toSquare))
        return;

    pseudoLegalMoves->push_back(Move(move));
}

int8_t Board::MoveNum() const {
    return moves.size();
}

Square Board::GetEnPassantSquare() const {
    if (moves.empty())
        return -1;

    return moves.top().EPSquare;
}

void Board::PushMove(const Move &move) {
    SoftPushMove(move);

    InspectBoard();
}

void Board::SoftPushMove(const Move &move) {
    moves.push(move);

    std::swap(board[move.fromSquare], board[move.toSquare]);
    board[move.fromSquare] = EMPTY_SQUARE;

    turn = !turn;
}

Move Board::UndoMove() {
    Move undone = SoftUndoMove();

    auto ptr = pastMoveLists.top();
    pastMoveLists.pop();
    delete ptr;

    pseudoLegalMoves = pastMoveLists.top();

    return undone;
}

Move Board::SoftUndoMove() {
    Move undone = moves.top();
    moves.pop();

    std::swap(board[undone.fromSquare], board[undone.toSquare]);
    board[undone.toSquare] = undone.capturedPiece;

    return undone;
}

std::list<Move>* Board::GetPseudoLegalMoves() const {
    return pseudoLegalMoves;
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

//void Board::PushMove(const std::string& move) {
//    for (const Move& legalMove : legalMoves) {
//        if (legalMove != move) continue;
//
//        SoftPushMove(legalMove);
////        moveJudge.InspectBoard();
//        return;
//    }
//
//    PrintBoard();
//
//    std::cout << "Could not find move: " << move << std::endl;
//
//    PrintMoveStack();
//}
//
//void Board::SoftPushMove(const Board::Move &move) {
//    moveStack.push(move);
//}
//
//std::string Board::UndoMove() {
//    std::string move = SoftUndoMove();
//
////    moveJudge.InspectBoard();
//
//    return move;
//}
//
//std::string Board::SoftUndoMove() {
//    if (moves.empty()) {
//        std::cout << "No moves to undo" << std::endl;
//        return "";
//    }
//
//    std::string move = moves.top();
//    moves.pop();
//
//    int fromIndex, toIndex;
//    char promotion;
//
//    char capturedPiece = ParseStdMove(move, fromIndex, toIndex, promotion);
//
//    if (move == "e1g1" && board[fromIndex] == 'K') {
//        std::swap(board[fromIndex], board[toIndex]);
//        std::swap(board[Index("a1")], board[toIndex + 1]);
//    }
//    else if (move == "e1c1" && board[fromIndex] == 'K') {
//        std::swap(board[fromIndex], board[toIndex]);
//        std::swap(board[Index("h1")], board[toIndex - 1]);
//    }
//    else if (move == "e8g8" && board[fromIndex] == 'k') {
//        std::swap(board[fromIndex], board[toIndex]);
//        std::swap(board[Index("a8")], board[toIndex + 1]);
//    }
//    else if (move == "e8c8" && board[fromIndex] == 'k') {
//        std::swap(board[fromIndex], board[toIndex]);
//        std::swap(board[Index("h8")], board[toIndex - 1]);
//    } else goto normal_move;
//
//    for (int & castling_right : castling_rights) {
//        if (castling_right == moves.size())
//            castling_right = castle_legal;
//    }
//
//    turn = !turn;
//    return move;
//
//    normal_move:
//    std::swap(board[toIndex], board[fromIndex]);
//
//    board[toIndex] = capturedPiece;
//
//    turn = !turn;
//
//    if (promotion != empty_square) {
//        board[fromIndex] = turn ? 'P' : 'p';
//    }
//
//    int minus = move.find('-');
//
//    if (minus != std::string::npos) {
//        std::string square = move.substr(minus + 1, 2);
//
//        board[Index(square)] = turn ? 'p' : 'P';
//    }
//
//    if (fromIndex == Index("e1") && board[fromIndex] == 'K' && castling_rights[0] == moves.size())
//        castling_rights[0] = castle_legal;
//    else if (fromIndex == Index("e1") && board[fromIndex] == 'K' && castling_rights[1] == moves.size())
//        castling_rights[1] = castle_legal;
//    else if (fromIndex == Index("e8") && board[fromIndex] == 'k' && castling_rights[2] == moves.size())
//        castling_rights[2] = castle_legal;
//    else if (fromIndex == Index("e8") && board[fromIndex] == 'k' && castling_rights[3] == moves.size())
//        castling_rights[3] = castle_legal;
//
//    return move;
//}
//
//
//char Board::ParseStdMove(std::string move, int &fromIndex, int &toIndex, char& promotion) {
//    std::string fromSquare = move.substr(0, 2);
//    std::string toSquare = move.substr(2, 2);
//    char capture = empty_square;
//    promotion = empty_square;
//
//    fromIndex = Index(fromSquare);
//    toIndex = Index(toSquare);
//
//    for (int i = 0; i < move.length() - 1; i++) {
//        if (move[i] == 'x')
//            capture = move[i + 1];
//        if (move[i] == '=')
//            promotion = move[i + 1];
//    }
//
//    return capture;
//}
//
//
//
//void Board::PrintAttackedSquares() {
//std::cout << "------------------------" << std::endl;
//    for (int i = 0; i < 64; i++) {
//        std::cout << attacked_squares[i] << " ";
//        if (i % 8 == 7) {
//            std::cout << std::endl;
//        }
//    }
//    std::cout << "------------------------" << std::endl;
//}
//
////void Board::GenPseudoLegalMoves(std::list<Move> &list) {
////
////}
////
////void Board::GenAttackedSquares(std::array<int, 64> &squaresRef) {
////    turn = !turn;
////    std::list<std::string> list;
////    //GenPseudoLegalMoves(list);
////    turn = !turn;
////
////    for (std::string move : list) {
////        int fromIndex, toIndex;
////        char promotion;
////        ParseStdMove(move, fromIndex, toIndex, promotion);
////
////        if (tolower(board[fromIndex]) != 'p')
////            squaresRef[toIndex]++;
////    }
////
////    for (int i = 0; i < 64; i++) {
////        // If the square is a pawn, attack its diagonals
////        if (!turn) {
////            if (board[i] == 'P') {
////                if (i % 8 != 0)
////                    squaresRef[i - 9]++;
////                if (i % 8 != 7)
////                    squaresRef[i - 7]++;
////            }
////        } else {
////            if (board[i] == 'p') {
////                if (i % 8 != 0)
////                    squaresRef[i + 7]++;
////                if (i % 8 != 7)
////                    squaresRef[i + 9]++;
////            }
////        }
////    }
////}
////
////void Board::PruneNonKosherMoves(std::list<std::string> &non_pruned, std::list<std::string> &pruned) {
////    std::string squareIgnore;
////    std::string squareAutoAdd;
////    int* tempPointer = new int;
////
////    for (const std::string& move : non_pruned) {
////
////        std::string square = move.substr(0, 2);
////
////        if (square == squareIgnore)
////            continue;
////        else if (square == squareAutoAdd) {
////            pruned.push_back(move);
////            continue;
////        }
////
////        int fromIndex, toIndex;
////        char promotion;
////
////        ParseStdMove(move, fromIndex, toIndex, promotion);
////
////        if (Ally(toIndex))
////            continue;
////
////        if (tolower(board[fromIndex]) == 'k') {
////            // If the player is trying to move the king, check if the square is attacked
////            if (attacked_squares[toIndex] > 0)
////                continue;
////
////            // Double check to see if the king was blocking the current square
////            if (RaycastReflect(toIndex, fromIndex, 1))
////                continue;
////
////            pruned.push_back(move);
////        } else {
////            // If the player is trying to move a non-king piece, make sure it does not put the king in check
////            int kingIndex = FindKing();
////            int attacked = attacked_squares[kingIndex];
////
////            switch(attacked) {
////                case 2:
////                    // If the king is in double-check, the king must move
////                    continue;
////                case 1:
////                    // If the king is in single-check, check to see if the piece is blocking or capturing
////                    if (RaycastReflect(kingIndex, toIndex, 0, tempPointer))
////                        if (closerSquare(kingIndex, *tempPointer, toIndex) == toIndex)
////                            pruned.push_back(move);
////                    continue;
////                case 0:
////                    // If the king is not in check, check to see if the piece is pinned
////                    if (!RaycastReflect(kingIndex, toIndex, 0, tempPointer)
////                        || closerSquare(kingIndex, *tempPointer, toIndex) == toIndex) {
////                        pruned.push_back(move);
////                        squareAutoAdd = square;
////                    } else {
////                        Board::GenPinnedMoves(pruned, fromIndex);
////                        squareIgnore = square;
////                    }
////                    break;
////                default:
////                    break;
////            }
////        }
////    }
////
////    delete(tempPointer);
////
//////    for (const std::string& move : non_pruned) {
//////        SoftPushMove(move);
//////
//////        if (!IsSoftCheck())
//////            pruned.push_back(move);
//////
//////        SoftUndoMove();
//////    }
////
////
////    GenCastling(pruned);
////}
//
//void Board::GenPinnedMoves(std::list<std::string> &list, int fromIndex) {
//    int kingIndex = FindKing();
//
//    int rankDiff = mmath::sign(Rank(fromIndex) - Rank(kingIndex));
//    int fileDiff = mmath::sign(File(fromIndex) - File(kingIndex));
//
//    int boardPiece = tolower(board[fromIndex]);
//    int sum = abs(rankDiff) + abs(fileDiff);
//
//    if (boardPiece != 'q' && !(sum == 2 && boardPiece == 'b') && !(sum == 1 && boardPiece == 'r')) return;
//
//    int diff = rankDiff * 8 + fileDiff;
//
//    for (int i = kingIndex; i >= 0 && i < 64; i += diff) {
//        if (i == fromIndex) continue;
//
//        if (Ally(i)) break;
//
//        list.push_back(Square(fromIndex) + Square(i) + (board[i] == ' ' ? "" : "x" + std::string(1, board[i])));
//
//        if (Enemy(i)) break;
//    }
//}
//
//void Board::GenCastling(std::list<std::string> &move_list) {
//    // Add Castling
//    if (turn && castling_rights[0] > moves.size() && SquareSafeAndEmpty("c1") && SquareSafeAndEmpty("d1") && !IsCheck()) {
//        AddMove(move_list, FindKing(), Index("c1"));
//    }
//    if (turn && castling_rights[1] > moves.size() && SquareSafeAndEmpty("f1") && SquareSafeAndEmpty("g1") && !IsCheck()) {
//        AddMove(move_list, FindKing(), Index("g1"));
//    }
//    if (!turn && castling_rights[2] > moves.size() && SquareSafeAndEmpty("c8") && SquareSafeAndEmpty("d8") && !IsCheck()) {
//        AddMove(move_list, FindKing(), Index("c8"));
//    }
//    if (!turn && castling_rights[3] > moves.size() && SquareSafeAndEmpty("f8") && SquareSafeAndEmpty("g8") && !IsCheck()) {
//        AddMove(move_list, FindKing(), Index("g8"));
//    }
//}
//
////void Board::GenCaptures(std::list<std::string> &move_list, std::list<std::string> &capture_list) {
////    for (const std::string& move : move_list) {
////        int fromIndex, toIndex;
////        char g;
////
////        ParseStdMove(move, fromIndex, toIndex, g);
////
////        if (board[toIndex] != empty_square) {
////            capture_list.push_back(move);
////        }
////    }
////}
//
//void Board::ParseKing(std::list<Move> &move_list, int index) {
//    int rank = Rank(index);
//    int file = File(index);
//
//    for (int x = -1; x <= 1; x++) {
//        for (int y = -1; y <= 1; y++) {
//            int xx = file + x;
//            int yy = rank + y;
//
//            if (xx < 0 || xx > 7 || yy < 0 || yy > 7 || (x == 0 && y == 0) || Ally(yy, xx))
//                continue;
//
//            AddMove(move_list, index, Index(yy, xx));
//        }
//    }
//}
//
//
//void Board::ParseQueen(std::list<Move> &move_list, int index) {
//    ParseBishop(move_list, index);
//    ParseRook(move_list, index);
//}
//
//void Board::ParseRook(std::list<Move> &move_list, int index) {
//    int delta = index % 8;
//
//    const int directions[] = {
//            1, -1, 8, -8
//    };
//    const int bounds[] = {
//            index + 7 - delta, index - delta, 64 - delta, 7 - delta
//    };
//
//    for (int i = 0; i < 4; i++) {
//        int off = directions[i];
//        int bound = bounds[i];
//        for (int dI = index + off; mmath::SidedCompare(dI, bounds[i], off < 0); dI += off) {
//            if (Ally(dI))
//                break;
//
//            AddMove(move_list, index, dI);
//
//            if (board[dI] != empty_square)
//                break;
//        }
//    }
//}
//
int8_t Board::Rank(int8_t index) {
    return 7 - index / 8;
}

int8_t Board::File(int8_t index) {
    return index % 8;
}
//
//std::string Board::Square(int index) {
//    return std::string(1, 'a' + File(index)) + std::string(1, Rank(index) + '1');
//}
//
//std::string Board::Square(int rank, int file) {
//    return Square(Index(rank, file));
//}
//
//
//bool Board::NotAlly(int index) {
//    return Empty(index) || Enemy(index);
//}
//
//bool Board::Ally(int rank, int file) {
//    return Ally(Index(rank, file));
//}
//
//bool Board::Ally(int index) {
//    return !Empty(index) && isupper(board[index]) == turn;
//}
//
//bool Board::Enemy(int index) {
//    return !Empty(index) && isupper(board[index]) != turn;
//}
//
//bool Board::Enemy(int rank, int file) {
//    if (rank < 0 || rank > 7 || file < 0 || file > 7)
//        return false;
//
//    return Enemy(Index(rank, file));
//}
//
//bool Board::Empty(int index) {
//    return index != -1 && board[index] == empty_square;
//}
//
//bool Board::Empty(int rank, int file) {
//    if (rank < 0 || rank > 7 || file < 0 || file > 7)
//        return false;
//
//    return board[Index(rank, file)] == empty_square;
//}
//
//bool Board::IsCheck() {
//    return attacked_squares[FindKing()];
//}
//
//std::string Board::GenMove(int fromSquare, int toSquare, char promotion, bool en_passant) {
//    std::string baseMove = Square(fromSquare) + Square(toSquare);
//    std::string capture = (board[toSquare] == empty_square) ? "" : "x" + std::string(1, board[toSquare]);
//    std::string prom = (promotion == 0) ? "" : ("=" + std::string(1, promotion));
//
//    int diff = fromSquare - toSquare;
//
//    std::string enpass = en_passant ? "-" + Square(fromSquare + mmath::sign(diff) * 8 - diff) : "";
//
//    return baseMove + capture + prom + enpass;
//}
//
//void Board::AddMove(std::list<std::string>& move_list, int fromSquare, int toSquare, char promotion, bool en_passant) {
//    if (toSquare == -1 || fromSquare == -1)
//        return;
//
//    std::string move = GenMove(fromSquare, toSquare, promotion, en_passant);
//
//    if (fromSquare < 0 || fromSquare > 63 || toSquare < 0 || toSquare > 63)
//        return;
//
//    move_list.push_back(move);
//}
//
////void Board::AddMove(std::list<Move>& move_list, int fromSquare, int toSquare, char promotion, bool en_passant) {
////    MoveState state = NORMAL;
////
////    if (promotion)
////        state = PROMOTION;
////    else if (en_passant)
////        state = ENPASSANT;
////    else if (board[toSquare] != empty_square)
////        state = CAPTURE;
////
////    move_list.push_back({state, fromSquare, toSquare, board[toSquare], promotion});
////}
//
//char Board::GetPieceAt(int rank, int file) const {
//    return board[Index(rank, file)];
//}
//
int8_t Board::Index(std::string square) {
    return (square[0] - 'a') + (7 - (square[1] - '1')) * 8;
}
//
//void Board::PrintMoveStack() {
//    std::stack<std::string> temp;
//
//    while (!moves.empty()) {
//        temp.push(moves.top());
//        moves.pop();
//    }
//
//    while (!temp.empty()) {
//        std::cout << temp.top() << std::endl;
//        moves.push(temp.top());
//        temp.pop();
//    }
//}
//
//bool Board::RaycastReflect(int from, int aim, int ignores, int *pointTouched) {
//    return RaycastReflectToPoint(from, aim, -1, ignores, pointTouched);
//}
//
//bool Board::RaycastReflectToPoint(int from, int aim, int square, int ignores, int *pointTouched) {
//    int rankDiff = Rank(aim) - Rank(from);
//    int fileDiff = File(aim) - File(from);
//
//    // if the direction of the ray is not diagonal or straight, return false
//    if (abs(rankDiff) != abs(fileDiff) && rankDiff != 0 && fileDiff != 0)
//        return false;
//
//    rankDiff = mmath::sign(rankDiff);
//    fileDiff = mmath::sign(fileDiff);
//
//    bool diagonal = abs(rankDiff) == abs(fileDiff);
//
//    int rank = Rank(from) + rankDiff;
//    int file = File(from) + fileDiff;
//
//    while (rank >= 0 && rank <= 7 && file >= 0 && file <= 7 && Index(rank, file) != square) {
//        if (!Empty(rank, file))
//            if (Ally(rank, file)) {
//                if (ignores > 0)
//                    ignores--;
//                else {
//                    if (pointTouched != nullptr)
//                        *pointTouched = Index(rank, file);
//
//                    return false;
//                }
//            } else if (Enemy(rank, file))
//                if (tolower(GetPieceAt(rank, file)) == 'r' && !diagonal
//                       || tolower(GetPieceAt(rank, file)) == 'b' && diagonal
//                       || tolower(GetPieceAt(rank, file)) == 'q') {
//                    if (pointTouched != nullptr)
//                        *pointTouched = Index(rank, file);
//
//                    return true;
//                } else
//                    return false;
//
//        rank += rankDiff;
//        file += fileDiff;
//    }
//
//    if (pointTouched != nullptr)
//        *pointTouched = -1;
//
//    return false;
//}
//
////bool Board::IsSoftCheck() {
////    if (moves.empty())
////        throw std::runtime_error("IsSoftCheck() called without a move on the stack");
////
////    int from, to;
////    char prom;
////
////    ParseStdMove(moves.top(), from, to, prom);
////
////    // If the player moved the king to a square that is attacked, that will always be a check
////    if (board[to] == (!turn ? 'K' : 'k'))
////        return attacked_squares[to] || RaycastReflect(to, from) || RaycastReflect(from, to, 1);
////
////    int kingIndex = FindEnemyKing();
////
////    switch (attacked_squares[kingIndex]) {
////        case 2:
////            // Case 1: The king is in double check, so the king needs to be moved
////            return true;
////        case 1:
////            // Case 2: The square was under attack before once, check to see if the move was a block.
////            return RaycastReflect(kingIndex, to) || !RaycastReflect(kingIndex, to, 1);
////        case 0:
////            // Case 3: The square was not attacked before, check to make sure the piece that was moved was not pinned.
////            return RaycastReflect(kingIndex, from);
////    }
////
////    // I don't think this should ever happen, but just in case
////    // I hate that GitHub CoPilot generated that above comment, wtf
////    return true;
////}
//
//bool Board::SquareSafeAndEmpty(const std::string& square) {
//    return !attacked_squares[Index(square)] && board[Index(square)] == empty_square;
//}
//
//bool Board::IsStalemate() {
//    return !IsCheck() && legalMoves.empty();
//}
//
//bool Board::IsCheckmate() {
//    return IsCheck() && legalMoves.empty();
//}
//
//bool Board::IsGameEnd() {
//    return IsCheckmate() || IsStalemate() || moves.size() >= 100;
//}
//
//int Board::FindKing() {
//    for (int i = 0; i < 64; i++) {
//        if (board[i] == 'k' && !turn || board[i] == 'K' && turn)
//            return i;
//    }
//
//    return -1;
//}
//
//int Board::FindEnemyKing() {
//    for (int i = 0; i < 64; i++) {
//        if (board[i] == 'k' && turn || board[i] == 'K' && !turn)
//            return i;
//    }
//
//    return -1;
//}
//
//bool Board::IsAttacked(int index) {
//    return attacked_squares[index];
//}
//
//char Board::GetPieceAt(int x) const {
//    return board[x];
//}
//
//char Board::GetPieceAt(std::string square) const {
//    return board[Index(std::move(square))];
//}
//
//bool Board::GetTurn() {
//    return turn;
//}
//
///**
// * @brief Determines which square is closer to the base square. This does assume that both compare squares are on the same diagonal/rank/file as each other and the base square.
// * @param base
// * @param compare1
// * @param compare2
// * @return
// */
//int Board::closerSquare(int base, int compare1, int compare2) {
//    int r = Rank(base);
//    int f = File(base);
//
//    int dif1 = abs(r - Rank(compare1)) + abs(f - File(compare1));
//    int dif2 = abs(r - Rank(compare2)) + abs(f - File(compare2));
//
//    if (dif1 > dif2)
//        return compare2;
//    else
//        return compare1;
//}
