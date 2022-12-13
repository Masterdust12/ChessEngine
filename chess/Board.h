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

class Board {
public:
    std::array<char, 64> board{};
    int castling_rights[4] = {0, 0, 0, 0};
    bool turn;

    Board();

    Board(std::string FEN, bool fauxStart = false, bool debug = false);

    ~Board();

    int8_t GetPieceAt(int8_t index) const;
    bool IsPieceAt(int8_t index, char piece) const;
    void SetPieceAt(int8_t index, char piece);

    int8_t GetPieceAt(int8_t index, int8_t fileOffset, int8_t rankOffset) const;
    bool IsPieceAt(int8_t index, int8_t fileOffset, int8_t rankOffset, char piece) const;
    void SetPieceAt(int8_t index, int8_t fileOffset, int8_t rankOffset, char piece);

    bool WhitePiece(int8_t index) const;
    bool PieceTurnRelation(int8_t index) const;

    bool Empty(int8_t index) const;
    bool Ally(int8_t index) const;
    bool Enemy(int8_t index) const;

    bool Empty(int8_t index, int8_t fileOffset, int8_t rankOffset) const;
    bool Ally(int8_t index, int8_t fileOffset, int8_t rankOffset) const;
    bool Enemy(int8_t index, int8_t fileOffset, int8_t rankOffset) const;

    void AddPseudoLegalMove(const Move& move);

    int8_t MoveNum() const;

    int8_t GetEnPassantSquare() const;
    std::list<Move>* GetPseudoLegalMoves() const;


    static int8_t Index(std::string square);
    static int8_t Rank(int8_t index);
    static int8_t File(int8_t index);

    void PushMove(const Move &move);
    void SoftPushMove(const Move &move);
    Move UndoMove();
    Move SoftUndoMove();

    bool IsAttacked(int8_t index);

    std::list<Move> GetLegalMoves();

    std::list<std::string> GetLegalCaptures();

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

    static std::string Square(int8_t index);
    static std::string Square(char rank, char file);

private:
    std::stack<Move> moves;
    std::stack<std::list<Move>*> pastMoveLists;

    std::list<Move>* pseudoLegalMoves;
    std::list<Move>* legalMoves;

    std::stack<Move> moveStack;

    std::map<char, struct Square*> m_EPMap;

    void InspectBoard();

    static int closerSquare(int base, int compare1, int compare2);

    void GenCastling(std::list<std::string> &list);

    void GenPinnedMoves(std::list<std::string> &list, int fromIndex);

    void ParseFEN(std::string FEN);

    void AddMove(std::list<std::string> &move_list, int fromSquare, int toSquare, char promotion = 0,
                 bool en_passant = false);

    void AddMove(std::list<Move> &move_list, int fromSquare, int toSquare, char promotion = 0, bool en_passant = false);
};

#endif //CHESSENGINE_BOARD_H
