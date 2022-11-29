#ifndef CHESSENGINE_BOARD_H
#define CHESSENGINE_BOARD_H

#include <string>
#include <stack>
#include <list>
#include <array>

class Board {

public:
    std::array<char, 64> board{};

    Board();
    explicit Board(std::string FEN, bool fauxStart = false, bool debug = false);
    void PushMove(const std::string& move);
    void SoftPushMove(const std::string& move);

    std::string UndoMove();
    std::string SoftUndoMove();

    char GetPieceAt(int x, int y) const;
    char GetPieceAt(int x) const;
    char GetPieceAt(std::string square) const;

    bool IsAttacked(int index);

    std::list<std::string> GetLegalMoves();
    std::list<std::string> GetLegalCaptures();
    std::list<std::string> GetPseudoLegalMoves();
    std::list<std::string> GetPseudoLegalCaptures();

    int FindEnemyKing();
    int FindKing();

    bool IsCheck();
    bool IsSoftCheck();
    bool IsStalemate();
    bool IsCheckmate();
    bool IsGameEnd();

    bool GetTurn();

    bool SquareSafeAndEmpty(const std::string& square);

    bool RaycastReflect(int from, int aim, int ignores = 0, int* pointTouched = nullptr);
    bool RaycastReflectToPoint(int from, int aim, int stopSquare, int ignores = 0, int* pointTouched = nullptr);

    void PrintBoard();
    void PrintAttackedSquares();
    void PrintMoveStack();
    std::string GenMove(int fromSquare, int toSquare, char promotion = 0, bool en_passant = false);

    bool turn;

    bool NotAlly(int index);

    bool Ally(int rank, int file);
    bool Ally(int index);
    bool Enemy(int index);
    bool Enemy(int rank, int file);
    bool Empty(int index);
    bool Empty(int rank, int file);

    static std::string Square(int index);
    static std::string Square(int rank, int file);

    static int Index(int x, int y);
    static int Index(std::string square);

    static int Rank(int index);
    static int File(int index);

    static char ParseMove(std::string move, int &fromIndex, int &toIndex, char &promotion) ;

private:
    int castling_rights[4] = { 0, 0, 0, 0 };
    std::string soft_move;

    std::array<int, 64> attacked_squares = { false };
    std::stack<std::string> moves;
    std::list<std::string> pseudoLegalMoves;
    std::list<std::string> pseudoLegalCaptures;
    std::list<std::string> legalMoves;
    std::list<std::string> legalCaptures;

    void Inspect();
    void GenPseudoLegalMoves(std::list<std::string> &list);
    void GenCaptures(std::list<std::string> &move_list, std::list<std::string> &capture_list);
    void PruneNonKosherMoves(std::list<std::string> &non_pruned, std::list<std::string> &pruned);
    void GenAttackedSquares(std::array<int, 64> &squaresRef);

    void ParseBishop(std::list<std::string> &list, int index);
    void ParseKing(std::list<std::string> &list, int index);
    void ParseKnight(std::list<std::string> &list, int index);
    void ParsePawn(std::list<std::string> &list, int index);
    void ParseQueen(std::list<std::string> &list, int index);
    void ParseRook(std::list<std::string> &list, int index);

    static int closerSquare(int base, int compare1, int compare2);

    void GenCastling(std::list<std::string> &list);
    void GenPinnedMoves(std::list<std::string> &list, int fromIndex);

    void ParseFEN(std::string FEN);
    void AddMove(std::list<std::string> &move_list, int fromSquare, int toSquare, char promotion = 0, bool en_passant = false);
};


#endif //CHESSENGINE_BOARD_H
