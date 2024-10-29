#pragma once

#include "Board.h"


class Game {
  /* A class that unifies game state and logic */

 private:
  void invalidateAllLegalMoves();
  void nextTurn() { board.isWhiteTurn = !board.isWhiteTurn; }

 public:
  Board board;
  Game();
  ~Game() = default;
  std::shared_ptr<Piece> getPieceByCoordinates(int row, int col);
  bool move(std::shared_ptr<Piece> piece, int row, int col);
  const void isCheck();

  Board::Iterator begin();
  Board::Iterator end();
};
