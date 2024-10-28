#pragma once

#include "Board.h"
#include "Piece.h"

class Game {
  /* A class that unifies game state and logic */

 private:
  bool isCheck = false;
  std::shared_ptr<Piece> whiteKing;
  std::shared_ptr<Piece> blackKing;

  void initializeBoard();
  void invalidateAllLegalMoves();
  void nextTurn() { isWhiteTurn = !isWhiteTurn; }
  void setIsCheck(Piece& piece);

 public:
  bool isWhiteTurn = true;
  Board board;
  Game();
  ~Game() = default;
  std::shared_ptr<Piece> getPieceByCoordinates(int row, int col);
  bool move(std::shared_ptr<Piece> piece, int row, int col);

  Board::Iterator begin();
  Board::Iterator end();
};
