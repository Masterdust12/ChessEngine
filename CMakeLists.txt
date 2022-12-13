project(ChessEngine)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(chess)

add_executable(ChessEngine
        chess/Board.cpp
        chess/BoardMove.cpp
        chess/MoveJudge.cpp
        chess/PieceParser.cpp
        engine/Engine.cpp
        main.cpp chess/Square.h chess/Square.cpp)
