#ifndef CHESSENGINE_BOARD_H
#define CHESSENGINE_BOARD_H

// Board Macros:
#define BOARD_SIZE 64
#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

#define EMPTY_SQUARE '.'

#define CASTLE_LEGAL 999999

// Math Macros:
#define SidedCompare(a, b, c) a > b == c
#define Sign(a) (a > 0) - (a < 0)

#include <string>
#include <stack>
#include <list>
#include <array>
#include <map>
#include "BoardMove.h"

struct Square {
    int file, rank;

    Square(int index) {
        file = index % BOARD_WIDTH;
        rank = index / BOARD_WIDTH;
    }

    Square(int file, int rank) : file(file), rank(rank) {}

    operator char() const {
        if (rank < 0 || rank > 7 || file < 0 || file > 7)
            return -1;

        return (7 - rank) * 8 + file;
    }


    Square operator +(const Square& move) const {
        return {file + move.file, rank + move.rank};
    }
};

struct EPIndex {
    char moveNum;
    Square* square;

    ~EPIndex() {
        delete square;
    }
};

class Board {
public:
    std::array<char, 64> board{};
    int castling_rights[4] = {0, 0, 0, 0};
    bool turn;

    Board();

    Board(std::string FEN, bool fauxStart = false, bool debug = false);

    char GetPieceAt(char index) const;
    bool IsPieceAt(char index, char piece) const;
    void SetPieceAt(char index, char piece);

    bool WhitePiece(char index) const;
    bool PieceTurnRelation(char index) const;

    bool Empty(char index) const;
    bool Ally(char index) const;
    bool Enemy(char index) const;

    void AddPseudoLegalMove(const Move& move);

    char MoveNum() const;

    Square* GetEnPassantSquare();
    void AddEnPassantSquare(char move, Square square);

    void PushMove(const std::string &move);

    void SoftPushMove(const Move &move);

    std::string UndoMove();

    std::string SoftUndoMove();

    bool IsAttacked(char index);

    std::list<Move> GetLegalMoves();

    std::list<std::string> GetLegalCaptures();

    std::list<Move> GetPseudoLegalMoves();

    std::list<std::string> GetPseudoLegalCaptures();

    int FindEnemyKing();

    int FindKing();

    bool IsCheck();

    bool IsStalemate();

    bool IsCheckmate();

    bool IsGameEnd();

    bool GetTurn();

    bool SquareSafeAndEmpty(const std::string &square);

    bool RaycastReflect(int from, int aim, int ignores = 0, int *pointTouched = nullptr);

    bool RaycastReflectToPoint(int from, int aim, int stopSquare, int ignores = 0, int *pointTouched = nullptr);

    void PrintBoard();

    void PrintAttackedSquares();

    void PrintMoveStack();

    std::string GenMove(char fromSquare, char toSquare, char promotion = 0, bool en_passant = false);

    bool Enemy(char index);

    bool Empty(char index);

    static std::string Square(char index);

    static std::string Square(char rank, char file);

    static char Index(char x, char y);

    static char Index(std::string square);

    static char Rank(char index);

    static char File(char index);

private:
    std::stack<std::string> moves;

    std::list<Move> pseudoLegalMoves;
    std::list<Move> legalMoves;

    std::stack<Move> moveStack;

    std::map<char, struct Square*> m_EPMap;
    EPIndex i_EPIndex;

    static int closerSquare(int base, int compare1, int compare2);

    void GenCastling(std::list<std::string> &list);

    void GenPinnedMoves(std::list<std::string> &list, int fromIndex);

    void ParseFEN(std::string FEN);

    void AddMove(std::list<std::string> &move_list, int fromSquare, int toSquare, char promotion = 0,
                 bool en_passant = false);

    void AddMove(std::list<Move> &move_list, int fromSquare, int toSquare, char promotion = 0, bool en_passant = false);
};

#endif //CHESSENGINE_BOARD_H
